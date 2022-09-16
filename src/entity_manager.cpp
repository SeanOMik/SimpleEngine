#include "entity_manager.h"

simpleengine::EntityManager::EntityManager() {

}

void simpleengine::EntityManager::submit_entity(std::shared_ptr<simpleengine::Entity> entity) {
    entities.emplace(entity->get_handle(), entity);
}

bool simpleengine::EntityManager::withdraw_entity(std::shared_ptr<simpleengine::Entity> entity) {
    auto it = entities.find(entity->get_handle());

    if (it != entities.end()) {
        it->second->destroy();
        entities.erase(it);

        return true;
    }

    return false;
}

void simpleengine::EntityManager::initialize() {

}

void simpleengine::EntityManager::destroy() {
    std::cout << "Destroy entity manager!" << std::endl;

    for (auto& [handle, entity] : entities) {
        entity->destroy();
    }
}

void simpleengine::EntityManager::update(const float& delta_time) {
    for (auto& [handle, entity] : entities) {
        entity->update(delta_time);
    }
}