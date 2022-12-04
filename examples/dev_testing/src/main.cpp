#include "simpleengine/camera.h"
#include "simpleengine/ecs/component/box_collider_component.h"
#include "simpleengine/ecs/component/mesh_component.h"
#include "simpleengine/ecs/component/rigid_body_component.h"
#include "simpleengine/ecs/component/transform_component.h"
#include "simpleengine/ecs/entity.h"
#include "simpleengine/ecs/registry.h"
#include "simpleengine/gfx/light.h"
#include "simpleengine/gfx/material.h"
#include "simpleengine/gfx/mesh.h"
#include "simpleengine/gfx/model.h"
#include "simpleengine/gfx/renderer.h"
#include "simpleengine/gfx/texture.h"
#include "simpleengine/vector.h"
#include <glm/gtx/string_cast.hpp>
#include <optional>
#include <semaphore.h>
#include <simpleengine/ecs/component/model_component.h>
#include <simpleengine/ecs/component/rotating_component.h>
#include <simpleengine/event/event.h>
#include <simpleengine/game.h>
#include <simpleengine/gfx/model.h>
#include <simpleengine/gfx/shader.h>
#include <simpleengine/gfx/shaders/core_3d_shader.h>
#include <simpleengine/renderable.h>
#include <simpleengine/shader_program.h>
#include <simpleengine/ecs/system/scene_system.h>
#include <simpleengine/vertex.h>
#include <simpleengine/log/logger.h>
#include <simpleengine/physics/physics_system.h>

#include <simpleengine/scripting/ecs_bindings.h>
#include <simpleengine/scripting/entt_meta_helper.h>

#include "entt/entity/fwd.hpp"

#include <assimp/material.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <sol/error.hpp>
#include <sol/load_result.hpp>
#include <spdlog/common.h>
#include <sstream>
#include <stdint.h>

#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

namespace se = simpleengine;

#define AUTO_ARG(x) decltype(x), x

class LuaTestScriptEvent : public se::Renderable {
public:
    sol::state lua;
    sol::protected_function_result script_res;
    entt::registry registry;

    static void lua_panic_handler(sol::optional<std::string> maybe_msg) {
        std::cerr << "Lua panic: ";
        if(maybe_msg) {
            std::cerr << maybe_msg->c_str();
        }
        std::cerr << std::endl;
    }

    static int lua_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
        std::cerr << "Lua Exception: ";
        if(maybe_exception) {
            const std::exception& ex = *maybe_exception;
            std::cerr << ex.what();
        } else {
            std::cerr.write(description.data(), description.size());
        }

        std::cerr << std::endl;

        return sol::stack::push(L, description);
    }

    static std::string lua_error_handler(lua_State* lstate, std::string msg) {
        std::cerr << "Lua error:  " << msg << std::endl;
        return msg;
    }

    static sol::protected_function_result lua_protected_function_handler(lua_State* lstate, sol::protected_function_result result) {
        std::cerr << "Lua protected function error" << std::endl;

        sol::type t = sol::type_of(lstate, result.stack_index());
		std::string err = "sol: ";
		err += to_string(result.status());
		err += " error";

		std::exception_ptr eptr = std::current_exception();
		if (eptr) {
			err += " with a ";
			try {
				std::rethrow_exception(eptr);
			}
			catch (const std::exception& ex) {
				err += "std::exception -- ";
				err.append(ex.what());
			}
			catch (const std::string& message) {
				err += "thrown message -- ";
				err.append(message);
			}
			catch (const char* message) {
				err += "thrown message -- ";
				err.append(message);
			}
			catch (...) {
				err.append("thrown but unknown type, cannot serialize into error message");
			}
		}

		if (t == sol::type::string) {
			err += ": ";
			std::string_view serr = sol::stack::unqualified_get<std::string_view>(lstate, result.stack_index());
			err.append(serr.data(), serr.size());
		}

        std::cerr << err << std::endl;
        /* sol::state_view lua(lstate);
        std::string traceback = lua.script("debug.traceback()");
        std::cout << "Traceback: " << traceback << std::endl; */

        return result;
    }

    LuaTestScriptEvent() : se::Renderable(), lua(sol::c_call<decltype(&LuaTestScriptEvent::lua_panic_handler),
            &LuaTestScriptEvent::lua_panic_handler>), registry({})/* , script_res(std::nullopt_t) */ {
        register_meta_component<se::ecs::TransformComponent>();

        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::debug);
        lua.require("registry", sol::c_call<AUTO_ARG(&open_registry)>, false); // create registry type
        // Overwrite the Lua print function to use a logger
        lua.globals().set_function("print", [](std::string msg) {
            SE_DEBUG("Lua ScriptingEngine", "{}", msg);
        });

        // Make registry available to lua
        lua["registry"] = std::ref(this->registry);

        // Registry TransformComponent
        lua.new_usertype<se::ecs::TransformComponent>("TransformComponent",
            "type_id", &entt::type_hash<se::ecs::TransformComponent>::value,

            sol::call_constructor, sol::factories(
                [](float px, float py, float pz) {
                    return se::ecs::TransformComponent(glm::vec3(px, py, pz));
                },
                [](float px, float py, float pz, float rx, float ry, float rz) {
                    return se::ecs::TransformComponent(glm::vec3(px, py, pz), glm::vec3(rx, ry, rz));
                },
                [](float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz) {
                    return se::ecs::TransformComponent(glm::vec3(px, py, pz), glm::vec3(rx, ry, rz), glm::vec3(sx, sy, sz));
                }
            ),

            sol::meta_function::to_string, [](const se::ecs::TransformComponent& self) {
                return glm::to_string(self.transform_matrix);
            } //&Transform::to_string
        );

	    lua.set_exception_handler(&LuaTestScriptEvent::lua_exception_handler);

        try {
            script_res = lua.safe_script(R"LUA(
                print('start')
                local dog = registry:create()
                local cat = registry:create()
                print('created cat and cat')

                print('Dog is ' .. dog .. ', and registry size is ' .. registry:size())
                print('Cat is ' .. cat .. ', and cat size is ' .. registry:size())

                assert(dog == 0 and cat == 1 and registry:size() == 2)

                registry:emplace(dog, TransformComponent(5, 6, 3))

                assert(registry:has(dog, TransformComponent))
                assert(registry:has(dog, TransformComponent.type_id()))

                assert(not registry:any_of(dog, -1, -2, -3))

                function update(delta_time)
                    transform = registry:get(dog, TransformComponent)
                    print('Dog position = ' .. tostring(transform))
                end

                print('Lua script loaded!')
            )LUA", &LuaTestScriptEvent::lua_protected_function_handler);
        } catch (sol::error e) {
            std::cerr << "Ran into sol2 error: " << e.what() << std::endl;
        } catch (std::exception e) {
            std::cerr << "Ran into std::exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Ran into something :shrug:" << std::endl;
        }
    }

    virtual void update(const float &delta_time) {
        if (script_res.valid()) {
            sol::function lua_update = lua["update"];
            
            if (lua_update.valid()) {
                lua_update(delta_time);
            }
        }
    }

    virtual void input_update(const float &delta_time) {
        if (script_res.valid()) {
            sol::function lua_input_update = lua["input_update"];
            
            if (lua_input_update.valid()) {
                lua_input_update(delta_time);
            }
        }
    }

    virtual void render(const float& interpolate_alpha, const float& frame_time) {
        if (script_res.valid()) {
            sol::function lua_render = lua["render"];

            if (lua_render.valid()) {
                lua_render(interpolate_alpha, frame_time);
            }
        }
    }
};

class FPSCounterEvent : public se::Renderable {
public:
    double last_frame_time_input;
    int frame_count_input;

    double last_frame_time_tps;
    int frame_count_tps;

    double last_frame_time_render;
    int frame_count_render;

    FPSCounterEvent() : se::Renderable() {
        last_frame_time_input = glfwGetTime();
        frame_count_input = 0;

        last_frame_time_tps = glfwGetTime();
        frame_count_tps = 0;

        last_frame_time_render = glfwGetTime();
        frame_count_render = 0;
    }

    virtual void update(const float &delta_time) {
        double current_time = glfwGetTime();
        frame_count_tps++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time_tps >= 1.0) {
            SE_DEBUG("performance", "Fixed update: {}tps", frame_count_tps);
            frame_count_tps = 0;
            last_frame_time_tps += 1.0;
        }
    }

    virtual void input_update(const float &delta_time) {
        double current_time = glfwGetTime();
        frame_count_input++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time_input >= 1.0) {
            SE_DEBUG("performance", "Input:        {}tps", frame_count_input);
            frame_count_input = 0;
            last_frame_time_input += 1.0;

        }
    }

    virtual void render(const float& interpolate_alpha, const float& frame_time) {
        double current_time = glfwGetTime();
        frame_count_render++;

        // Check if the last print was 1 second ago.
        if (current_time - last_frame_time_render >= 1.0) {
            double ms_per_frame = 1000 / (double)frame_count_render;

            SE_DEBUG("performance", "Render:       {}fps, {:.2f}ms/frame", frame_count_render, ms_per_frame);
            SE_DEBUG("performance", "-------------------------------");
            frame_count_render = 0;
            last_frame_time_render += 1.0;
        }
    }
};

int main(int argc, char *argv[]) {
    se::Game game(640, 480, "SimpleEngine 3D OpenGL - Developer Testing", GLFW_OPENGL_CORE_PROFILE, 4, 4, false);

    se::log::LoggerManager::set_level(spdlog::level::trace);

    // Load core shaders from SimpleEngine resources
    se::gfx::shaders::Core3dShader core_shader;

    // Create an entity registry
    auto registry = std::make_shared<se::ecs::Registry>();

    auto camera = std::make_shared<se::Camera>(game.get_window(), core_shader, 70, glm::vec3(-6, 0, 0));
    //game.add_event(camera);

    // Create a renderer
    auto renderer = std::make_shared<se::gfx::Renderer>(game.get_window(), core_shader, camera);
    renderer->initialize();

    // Create a Scene and give it the renderer
    auto scene = std::make_shared<se::ecs::system::SceneSystem>(registry, renderer, camera);
    game.add_renderable(scene); 

    // Create a Physics System for handling the physics
    auto physics_sys = std::make_shared<se::physics::PhysicsSystem>(registry);
    game.add_event(physics_sys);

    /* se::ecs::Entity other_e = scene->create_entity();
    other_e.add_component<se::ModelComponent>("examples/dev_testing/resources/transparent_window.fbx",
        se::gfx::ModelProcessingFlags::MdlProcFlag_TRANSPARENT);
    auto &other_transform = other_e.add_component<se::TransformComponent>();
    other_transform.translate(5.f, 0.5f, 1.f);

    // Create an Entity in the Scene and add components to it.
    se::ecs::Entity entity = scene->create_entity();

    entity.add_component<se::ModelComponent>("examples/dev_testing/resources/transparent_window.fbx", 
        se::gfx::ModelProcessingFlags::MdlProcFlag_TRANSPARENT);

    auto &transform_comp = entity.add_component<se::TransformComponent>();
    transform_comp.translate(4.f, 0.f, 0.f); */

    se::ecs::Entity brick_e = scene->create_entity();
    //brick_e.add_component<se::ecs::ModelComponent>("examples/dev_testing/resources/bricks/bricks.fbx");
    registry->get_inner().emplace<se::ecs::ModelComponent>(brick_e.get_inner(), "examples/dev_testing/resources/bricks/bricks.fbx");
    brick_e.add_component<se::ecs::TransformComponent>(glm::vec3(6.f, 6.f, 0.f));
    brick_e.add_component<se::ecs::BoxColliderComponent>(1.f, 1.f, 1.f);
    brick_e.add_component<se::ecs::RigidBodyComponent>(1.f, se::Vectorf(6.f, 6.f, 0.1f));

    se::ecs::Entity floor = scene->create_entity();
    floor.add_component<se::ecs::ModelComponent>("examples/dev_testing/resources/ground/ground.fbx");
    floor.add_component<se::ecs::TransformComponent>(glm::vec3(6.f, -6.f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f));
    floor.add_component<se::ecs::BoxColliderComponent>(1.f, 1.f, 1.f);
    floor.add_component<se::ecs::RigidBodyComponent>(0.f, se::Vectorf(6.f, -6.f, 0.f));


    // Add TransformComponent using Lua
    //register_met


    auto light = std::make_shared<se::gfx::Light>(core_shader, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
    game.add_event(light);

    auto fps_counter = std::make_shared<FPSCounterEvent>();
    game.add_renderable(fps_counter);

    auto lua_script = std::make_shared<LuaTestScriptEvent>();
    game.add_renderable(lua_script);

    /* game.set_enable_vsync(false);
    game.set_fps_limit(100); */
    int res = game.run();

    SE_CINFO("Engine result: {}", res);

    renderer->destroy();
    scene->destroy();

    return res;
}