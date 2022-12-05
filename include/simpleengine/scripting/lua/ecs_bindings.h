#pragma once

#include "../entt_meta_helper.h"

#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/runtime_view.hpp>

#include <set>

#include <sol/sol.hpp>
#include <sol/state_view.hpp>
#include <sol/types.hpp>

namespace simpleengine::scripting::lua {
    class ECSBindings {
    private:
        template <typename Component> static auto is_valid(const entt::registry *registry, entt::entity entity) {
            assert(registry);
            return registry->valid(entity);
        }

        template <typename Component>
        static auto emplace_component(entt::registry *registry, entt::entity entity, const sol::table& instance, sol::this_state s) {
            assert(registry);
            auto& comp =
                registry->emplace_or_replace<Component>(entity, instance.valid() ? instance.as<Component>() : Component{});
            return sol::make_reference(s, std::ref(comp));
        }

        template <typename Component> static auto get_component(entt::registry *registry, entt::entity entity, sol::this_state s) {
            assert(registry);
            auto& comp = registry->get_or_emplace<Component>(entity);
            return sol::make_reference(s, std::ref(comp));
        }

        template <typename Component> static bool has_component(entt::registry *registry, entt::entity entity) {
            assert(registry);
            return registry->any_of<Component>(entity);
        }

        template <typename Component> static auto remove_component(entt::registry *registry, entt::entity entity) {
            assert(registry);
            return registry->remove<Component>(entity);
        }

        template <typename Component> static void clear_component(entt::registry *registry) {
            assert(registry);
            registry->clear<Component>();
        }

        template <typename Component> static void register_meta_component() {
            using namespace entt::literals;

            entt::meta<Component>()
                .type()
                .template func<&is_valid<Component>>("valid"_hs)
                .template func<&emplace_component<Component>>("emplace"_hs)
                .template func<&get_component<Component>>("get"_hs)
                .template func<&has_component<Component>>("has"_hs)
                .template func<&clear_component<Component>>("clear"_hs)
                .template func<&remove_component<Component>>("remove"_hs);
        }

        static auto collect_types(const sol::variadic_args& va) {
            std::set<entt::id_type> types;
            std::transform(va.cbegin(), va.cend(), std::inserter(types, types.begin()),
                        [](const auto& obj) { return EnttMetaHelper::deduce_type(obj); });
            return types;
        }

    public:
    
        /// This binds ALL of the ECS to Lua under the `ecs` table
        static sol::table bind_full_ecs(sol::this_state s);

        /// This binds an entt registry to lua. It binds the registry type as `ecs.registry`
        static sol::table bind_registry(sol::this_state s);
        /// Bind all components to Lua. This will put all components under `ecs.component.*`
        static sol::table bind_components(sol::this_state s);
    };
}