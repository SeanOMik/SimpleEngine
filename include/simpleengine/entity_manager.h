#pragma once

#include "ecs/entity.h"
#include "event/event.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace simpleengine {
    class EntityManager : public simpleengine::Event {
    public:
        std::unordered_map<uint32_t, std::shared_ptr<simpleengine::Entity>> entities;

        EntityManager();

        virtual void submit_entity(std::shared_ptr<simpleengine::Entity> entity);
        virtual bool withdraw_entity(std::shared_ptr<simpleengine::Entity> entity);

        virtual void initialize();
        virtual void destroy() override;

        virtual void update(const float& delta_time) override;
    };
}