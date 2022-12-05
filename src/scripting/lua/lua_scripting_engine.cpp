#include "scripting/lua/lua_scripting_engine.h"
#include "scripting/lua/ecs_bindings.h"
#include "scripting/entt_meta_helper.h"
#include "ecs/registry.h"
#include "log/logger.h"

#include "ecs/component/transform_component.h"
#include "scripting/lua/ecs_bindings.h"

#include <iostream>

#include <glm/gtx/string_cast.hpp>

#define AUTO_ARG(x) decltype(x), x

namespace simpleengine::scripting::lua {
    LuaScriptingEngine::LuaScriptingEngine(std::shared_ptr<ecs::Registry> entity_registry) : ScriptingEngine(entity_registry) {
        
    }

    void LuaScriptingEngine::lua_panic_handler(sol::optional<std::string> maybe_msg) {
        std::cerr << "Lua panic: ";
        if(maybe_msg) {
            std::cerr << maybe_msg->c_str();
        }
        std::cerr << std::endl;
    }

    int LuaScriptingEngine::lua_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
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

    sol::protected_function_result LuaScriptingEngine::lua_protected_function_handler(lua_State* lstate, sol::protected_function_result result) {
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

        SE_ERROR("Lua ScriptingEngine", "{}", err);

        return result;
    }

    void LuaScriptingEngine::setup_language() {
        std::cout << "Setup" << std::endl;

        lua = sol::state{sol::c_call<decltype(&LuaScriptingEngine::lua_panic_handler),
            &LuaScriptingEngine::lua_panic_handler>};
        lua.set_exception_handler(&LuaScriptingEngine::lua_exception_handler);

        

        // Open all libraries built into sol2. There is ffi and jit, but these are only available
        // when using LuaJIT.
        lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine,
            sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::debug,
            sol::lib::bit32, sol::lib::io, sol::lib::utf8);
        //lua.require("registry", sol::c_call<AUTO_ARG(&simpleengine::scripting::lua::bindings::bind_registry)>, false); // create registry type
        // Overwrite the Lua print function to use a logger
        lua.globals().set_function("print", [](std::string msg) {
            SE_DEBUG("Lua ScriptingEngine", "[Lua Output] {}", msg);
        });
    }
    
    void LuaScriptingEngine::expose_simpleengine() {
        // Make registry available to lua
        lua["registry"] = std::ref(entity_registry->get_inner());

        // Registry TransformComponent
        //bindings::bind_registry();
        lua.require("ecs", sol::c_call<AUTO_ARG(&simpleengine::scripting::lua::ECSBindings::bind_full_ecs)>, false); // create registry type
        //lua.require("registry", sol::c_call<AUTO_ARG(&simpleengine::scripting::lua::bindings::bind_registry)>, false); // create registry type


        entt::entity en = entity_registry->get_inner().create();
        lua["dog"] = en;

        run_script(R"LUA(
            print('start')
            --local dog = registry:create()
            local cat = registry:create()
            print('created dog and cat')

            print('Dog is ' .. dog .. ', and registry size is ' .. registry:size())
            print('Cat is ' .. cat .. ', and cat size is ' .. registry:size())

            registry:emplace(dog, ecs.component.TransformComponent(5, 6, 3))

            assert(registry:has(dog, ecs.component.TransformComponent))
            assert(registry:has(dog, ecs.component.TransformComponent.type_id()))

            assert(not registry:any_of(dog, -1, -2, -3))

            function update(delta_time)
                transform = registry:get(dog, ecs.component.TransformComponent)
                print('Dog position = ' .. tostring(transform))
                transform:translate(0, 0, 1)
            end

            print('Lua script loaded!')
        )LUA");
    }

    void LuaScriptingEngine::run_script(std::string lua_code) {
        script_results.emplace_back(lua.script(lua_code, &LuaScriptingEngine::lua_protected_function_handler));
    }

    void LuaScriptingEngine::run_script_file(std::string path) {
        script_results.emplace_back(lua.script_file(path, &LuaScriptingEngine::lua_protected_function_handler));
    }

    void LuaScriptingEngine::update(const float& delta_time) {
        for (const auto& res : script_results) {
            if (res.valid()) {
                sol::function lua_update = lua["update"];
                
                if (lua_update.valid()) {
                    lua_update(delta_time);
                }
            }
        }
    }

    void LuaScriptingEngine::input_update(const float& delta_time) {
        for (const auto& res : script_results) {
            if (res.valid()) {
                sol::function lua_input_update = lua["input_update"];
                
                if (lua_input_update.valid()) {
                    lua_input_update(delta_time);
                }
            }
        }
    }

    void LuaScriptingEngine::render(const float& interpolate_alpha, const float& frame_time) {
        for (const auto& res : script_results) {
            if (res.valid()) {
                sol::function lua_render = lua["render"];
                
                if (lua_render.valid()) {
                    lua_render(interpolate_alpha, frame_time);
                }
            }
        }
    }
    
    void LuaScriptingEngine::destroy() {
        std::cout << "Destroy" << std::endl;
    }
}