#include "scripting/lua/ecs_bindings.h"

#include "ecs/component/transform_component.h"
#include "scripting/entt_meta_helper.h"

#include <glm/detail/type_quat.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sol/overload.hpp>
#include <tuple>

namespace simpleengine::scripting::lua {
    sol::table ECSBindings::bind_full_ecs(sol::this_state s) {
        sol::table ecs_table = bind_registry(s);
        bind_components(s);

        return ecs_table;
    }

    sol::table ECSBindings::bind_registry(sol::this_state s) {
        sol::state_view lua{s};
        auto ecs_module = lua["ecs"].get_or_create<sol::table>();

        ecs_module.new_usertype<entt::runtime_view>("runtime_view", sol::no_constructor,
            "size_hint", &entt::runtime_view::size_hint, "contains",
            &entt::runtime_view::contains, "each",
            [](const entt::runtime_view& self, const sol::function& callback) {
                if (!callback.valid()) {
                    return;   
                }
                for (auto entity : self) {
                    callback(entity);
                }
            });

        using namespace entt::literals;

        ecs_module.new_usertype<entt::registry>(
            "registry", sol::meta_function::construct, sol::factories([] { return entt::registry{}; }),

            "size", &entt::registry::size,
            "alive", &entt::registry::alive,
            "valid", &entt::registry::valid, "current", &entt::registry::current,

            "create", [](entt::registry& self) { return self.create(); },
            "destroy", [](entt::registry& self, entt::entity entity) { return self.destroy(entity); },

            "emplace",
            [](entt::registry& self, entt::entity entity, const sol::table& comp, sol::this_state s) -> sol::object {
                if (!comp.valid())
                    return sol::lua_nil_t{};

                const auto maybe_any = EnttMetaHelper::invoke_meta_func(EnttMetaHelper::get_type_id(comp), "emplace"_hs, &self, entity, comp, s);
                return maybe_any ? maybe_any.cast<sol::reference>() : sol::lua_nil_t{};
            },
    
            "remove",
            [](entt::registry& self, entt::entity entity, const sol::object& type_or_id) {
                const auto maybe_any = EnttMetaHelper::invoke_meta_func(EnttMetaHelper::deduce_type(type_or_id), "remove"_hs, &self, entity);
                return maybe_any ? maybe_any.cast<size_t>() : 0;
            },

            "has",
            [](entt::registry& self, entt::entity entity, const sol::object& type_or_id) {
                const auto maybe_any = EnttMetaHelper::invoke_meta_func(EnttMetaHelper::deduce_type(type_or_id), "has"_hs, &self, entity);
                return maybe_any ? maybe_any.cast<bool>() : false;
            },

            "any_of",
            [](const sol::table& self, entt::entity entity, const sol::variadic_args& va) {
                const auto types = collect_types(va);
                const auto has = self["has"].get<sol::function>();
                return std::any_of(types.cbegin(), types.cend(),
                    [&](auto type_id) { return has(self, entity, type_id).template get<bool>(); });
            },

            "get",
            [](entt::registry& self, entt::entity entity, const sol::object& type_or_id, sol::this_state s) {
                const auto maybe_any = EnttMetaHelper::invoke_meta_func(EnttMetaHelper::deduce_type(type_or_id), "get"_hs, &self, entity, s);
                return maybe_any ? maybe_any.cast<sol::reference>() : sol::lua_nil_t{};
            },
            
            "clear",
            sol::overload(&entt::registry::clear<>, [](entt::registry& self, sol::object type_or_id) {
                EnttMetaHelper::invoke_meta_func(EnttMetaHelper::deduce_type(type_or_id), "clear"_hs, &self);
            }),

            "orphan", &entt::registry::orphan,

            "runtime_view",
            [](entt::registry& self, const sol::variadic_args& va) {
                const std::set<uint32_t> types = collect_types(va);

                entt::runtime_view view{};
                for (const auto& type : types) {
                    entt::sparse_set& storage = self.storage(type)->second;
                    view.iterate(storage);
                }

                return view;
            }
        );

        return ecs_module;
    }

    sol::table ECSBindings::bind_components(sol::this_state s) {
        sol::state_view lua{s};
        auto ecs_module = lua["ecs"].get_or_create<sol::table>();
        auto comp_module = ecs_module["component"].get_or_create<sol::table>();

        // TransformComponent
        register_meta_component<ecs::TransformComponent>();
        comp_module.new_usertype<simpleengine::ecs::TransformComponent>("TransformComponent",
            "type_id", &entt::type_hash<simpleengine::ecs::TransformComponent>::value,

            sol::call_constructor, sol::factories(
                [](float px, float py, float pz) {
                    return simpleengine::ecs::TransformComponent(glm::vec3(px, py, pz));
                },
                [](float px, float py, float pz, float rx, float ry, float rz) {
                    return simpleengine::ecs::TransformComponent(glm::vec3(px, py, pz), glm::vec3(rx, ry, rz));
                },
                [](float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz) {
                    return simpleengine::ecs::TransformComponent(glm::vec3(px, py, pz), glm::vec3(rx, ry, rz), glm::vec3(sx, sy, sz));
                }
            ),

            "decompose_matrix", [](const ecs::TransformComponent& self) {
                glm::vec3 pos, scale;
                glm::quat rot;

                self.decompose_matrix(pos, rot, scale);

                return std::tuple(pos, rot, scale);
            },

            // TODO: Implement glm::vec3
            /* "get_pos", &ecs::TransformComponent::get_pos,
            "get_scale", &ecs::TransformComponent::get_scale,
            "get_rotation_quat", &ecs::TransformComponent::get_rotation_quat, */
            // combine_transform(const glm::mat4& transform_matrix)
            // combine_transform(const TransformComponent& transformable)

            "translate", sol::overload([](ecs::TransformComponent& self, float x, float y, float z) {
                self.translate(x, y, z);
            }), // TODO: Implement glm::vec3 translate
            
            "rotate", &ecs::TransformComponent::rotate,
            "rotate_x", &ecs::TransformComponent::rotate_x,
            "rotate_y", &ecs::TransformComponent::rotate_y,
            "rotate_z", &ecs::TransformComponent::rotate_z,

            "scale", sol::overload([](ecs::TransformComponent& self, float scalar) {
                self.scale(scalar);
            }, [](ecs::TransformComponent& self, float x_scalar, float y_scalar, float z_scalar) {
                self.scale(glm::vec3(x_scalar, y_scalar, z_scalar));
            }), // TODO: Implement glm::vec3 scale

            sol::meta_function::to_string, [](const ecs::TransformComponent& self) {
                return glm::to_string(self.transform_matrix);
            }
        );

        return comp_module;
    }
}