#pragma once

#include "entt/entity/fwd.hpp"
#include <cstddef>
#include <entt/entt.hpp>
#include <utility>

namespace simpleengine::ecs {
    class Entity {
    private:
        entt::registry& registry;
        entt::entity inner;
    public:
        /* Entity(entt::entity entity) : inner(entity) {

        } */

        Entity(entt::registry& registry, entt::entity entity) : registry(registry), inner(entity) {

        }

        /**
         * @brief Checks if an identifier refers to a valid entity.
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a1795b1221d728f806319a685930f520d
         *
         * @return true if the identifier is valid
         * @return false otherwise
         */
        bool is_valid() const {
            return registry.valid(inner);
        }

        /**
         * @brief Returns the actual version for an identifier. 
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a199babc787d6baa6f7ccce761228a5f6
         *
         * @return entt::registry::version_type The version for the given identifier if valid, the tombstone version otherwise. 
         */
        entt::registry::version_type current_version() const {
            return registry.current(inner);
        }

        /**
         * @brief Releases an identifier.
         *
         * The version is updated and the identifier can be recycled at any time.
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a3d9cb2368384b0952cb54848e777359e
         *
         * @return entt::registry::version_type The version of the recycled entity.
         */
        entt::registry::version_type release() {
            return registry.release(inner);
        }

        /**
         * @brief Releases an identifier.
         *
         * The suggested version or the valid version closest to the suggested one is used instead of the implicitly generated version.
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#af9c919867fc93a7e1a2d0762ac3a9877
         *
         * @param version A desired version upon destruction.
         * @return entt::registry::version_type The version actually assigned to the entity.
         */
        entt::registry::version_type release(entt::registry::version_type version) {
            return registry.release(inner, version);
        }

        /**
         * @brief Destroys an entity and releases its identifier. 
         * 
         * @warning Adding or removing components to an entity that is being destroyed can result in undefined behavior. Attempting to use an invalid entity results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a7b2c0368d508a6af2d094a9fc592a4a0
         *
         * @return entt::registry::version_type The version of the recycled entity.
         */
        entt::registry::version_type destroy() {
            return registry.destroy(inner);
        }

        /**
         * @brief Destroys an entity and releases its identifier.
         * 
         * The suggested version or the valid version closest to the suggested one is used instead of the implicitly generated version.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#adc175a0d3bcf83a133c63890c674ceb3
         *
         * @param version A desired version upon destruction.
         * @return entt::registry::version_type The version actually assigned to the entity.
         */
        entt::registry::version_type destroy(entt::registry::version_type version) {
            return registry.destroy(inner, version);
        }

        /**
         * @brief Assigns the given component to an entity. 
         * 
         * The component must have a proper constructor or be of aggregate type.
         *
         * @warning Attempting to use an invalid entity or to assign a component to an entity that already owns it results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a4c9c0532972adc7a930a50888a97efdf
         *
         * @tparam Component Type of component to create. 
         * @tparam Args Types of arguments to use to construct the component. 
         * 
         * @param args Parameters to use to initialize the component. 
         * 
         * @return decltype(auto) A reference to the newly created component. 
         */
        template<typename Component, typename... Args>
        decltype(auto) add_component(Args&&... args) {
            return registry.emplace<Component>(inner, std::forward<Args>(args)...);
        }

        /**
         * @brief Assigns or replaces the given component for an entity.
         * 
         * The component must have a proper constructor or be of aggregate type.
         *
         * @warning Attempting to use an invalid entity or to assign a component to an entity that already owns it results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a3bbaebbb9365eef262453e42a8f7dc4b
         *
         * @tparam Component Type of component to assign or replace.
         * @tparam Args Types of arguments to use to construct the component. 
         * 
         * @param args Parameters to use to initialize the component. 
         * 
         * @return decltype(auto) A reference to the newly created component. 
         */
        template<typename Component, typename... Args>
        decltype(auto) add_or_replace_component(Args&&... args) {
            return registry.emplace_or_replace<Component>(inner, std::forward<Args>(args)...);
        }

        /**
         * @brief Replaces the given component for an entity. 
         *
         * The component must have a proper constructor or be of aggregate type.
         * 
         * @warning Attempting to use an invalid entity or to replace a component of an entity that doesn't own it results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a0501ff5a96a3421f3eec427bf5d62380
         *
         * @tparam Component Type of component to replace. 
         * @tparam Args Types of arguments to use to construct the component. 
         *
         * @param args Parameters to use to initialize the component. 
         *
         * @return decltype(auto) A reference to the component being replaced. 
         */
        template<typename Component, typename... Args>
        decltype(auto) replace_component(Args&&... args) {
            return registry.replace<Component>(inner, std::forward<Args>(args)...);
        }

        /**
         * @brief Patches the given component for an entity. 
         * 
         * The signature of the function should be equivalent to the following:
         * \code{.cpp}
         * void(Component &);
         * \endcode
         *
         * @note Empty types aren't explicitly instantiated and therefore they are never returned. However, this function can be used to trigger an update signal for them.
         *
         * @warning Attempting to use an invalid entity or to patch a component of an entity that doesn't own it results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a22454253689ff77e41326f849bfea976
         *
         * @tparam Component Type of component to patch. 
         * @tparam Func Types of the function objects to invoke. 
         * 
         * @param func Valid function objects. 
         *
         * @return decltype(auto) A reference to the patched component. 
         */
        template<typename Component, typename... Func>
        decltype(auto) patch(Func&&... func) {
            return registry.patch<Component>(inner, std::forward<Func>(func)...);
        }

        /**
         * @brief Removes the given components from an entity. 
         * 
         * @warning Attempting to use an invalid entity results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#aa66ca71c5eb6c2e1b3bde6ff86a268c7
         *
         * @tparam Component Type of component to remove. 
         * @tparam Other Other types of components to remove. 
         *
         * @return size_t The number of components actually removed. 
         */
        template<typename Component, typename... Other>
        size_t remove_components() {
            return registry.remove<Component, Other...>(inner);
        }

        /**
         * @brief Erases the given components from an entity. 
         * 
         * @warning Attempting to use an invalid entity or to erase a component from an entity that doesn't own it results in undefined behavior.
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#ad7a51ade7ff181d7aa0b35915d0a4f2b
         * 
         * @tparam Component Types of components to erase. 
         * @tparam Other Other types of components to erase. 
         */
        template<typename Component, typename... Other>
        void erase() {
            return registry.remove<Component, Other...>(inner);
        }

        /**
         * @brief Checks if an entity has all the given components.
         * 
         * @warning Attempting to use an invalid entity results in undefined behavior.
         *
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a40072eca607846f43f47d0e3f11dd196
         * 
         * @tparam Component Components for which to perform the check.
         * 
         * @return true if the entity has all the components
         * @return false otherwise
         */
        template<typename... Component>
        bool has_all_of() const {
            return registry.all_of<Component...>(inner);
        }

        /**
         * @brief Checks if an entity has at least one of the given components. 
         * 
         * @warning Attempting to use an invalid entity results in undefined behavior.
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a08f819276fd2d8b2b2b6d01357f61a42
         *
         * @tparam Component Components for which to perform the check. 
         * 
         * @return true if the entity has at least one of the given components
         * @return false otherwise
         */
        template<typename... Component>
        bool has_any_of() const {
            return registry.any_of<Component...>(inner);
        }

        /**
         * @brief Checks if an entity has components assigned. 
         * 
         * @link https://skypjack.github.io/entt/classentt_1_1basic__registry.html#a10f5e61d4cabab9e9c1e87edc30de551
         * 
         * @return true if the entity has no components assigned
         * @return false otherwise
         */
        bool has_any_components() const {
            return registry.orphan(inner);
        }
    };
}