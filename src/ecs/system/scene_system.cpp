#include "gfx/renderer.h"
#include "ecs/system/scene_system.h"
#include "ecs/component/transform_component.h"
#include "ecs/component/rotating_component.h"
#include "ecs/entity.h"
#include "ecs/registry.h"
#include "ecs/component/model_component.h"
#include "ecs/component/mesh_component.h"

#include <glm/gtx/string_cast.hpp>
#include <spdlog/common.h>
#include <stdexcept>

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>

using namespace simpleengine::ecs;

namespace simpleengine::ecs::system {

    SceneSystem::SceneSystem(std::shared_ptr<Registry> entity_registry, std::shared_ptr<gfx::Renderer> renderer, std::shared_ptr<Camera> camera)
            : System(entity_registry), renderer(renderer), camera(camera) {
        
    }

    ecs::Entity SceneSystem::create_entity() {
        return entity_registry->create_entity();
    }

    void SceneSystem::input_update(const float& delta_time) {
        camera->input_update(delta_time); // Update camera input
    }

    void SceneSystem::update(const float& delta_time) {
        // Update the last transform matrix 
        entity_registry->get_inner().view<TransformComponent>().each([this, &delta_time](TransformComponent& transform) {
            transform.last_transform_matrix = transform.transform_matrix;
        });

        // Rotate the model
        /* registry->view<TransformComponent, RotatingComponent>().each([this, &delta_time](TransformComponent& transform, RotatingComponent& rotating) {
            transform.rotate(rotating.rate * delta_time, rotating.rotation_axis);
        }); */
    }

    void SceneSystem::render(const float& interpolate_alpha, const float& frame_time) {
        // Is there a way these can be grouped?
        entity_registry->get_inner().view<TransformComponent, ModelComponent>().each([this](TransformComponent& transform, ModelComponent& model_component) {
            for (auto& mesh : model_component.model.meshes) {
                auto rendering_type = gfx::RenderingType::RendType_OPAQUE;
                if (mesh.material) {
                    rendering_type = mesh.material->rendering_type;
                }

                renderer->queue_job(gfx::RenderingJob(rendering_type, mesh, transform.last_transform_matrix, transform.transform_matrix));
            }
        });

        entity_registry->get_inner().view<TransformComponent, MeshComponent>().each([this](TransformComponent& transform, MeshComponent& mesh_component) {
            auto rendering_type = gfx::RenderingType::RendType_OPAQUE;
            if (mesh_component.mesh.material) {
                rendering_type = mesh_component.mesh.material->rendering_type;
            }

            renderer->queue_job(gfx::RenderingJob(rendering_type, mesh_component.mesh, transform.last_transform_matrix, transform.transform_matrix));
        });

        renderer->render(interpolate_alpha, frame_time);
    }

    void SceneSystem::destroy() {
        SE_DEBUG("scene", "Destroying Scene...");
        entity_registry->get_inner().view<ModelComponent>().each([this](ModelComponent& model_component) {
            for (auto& mesh : model_component.model.meshes) {
                mesh.destroy();
            }
        });

        entity_registry->get_inner().view<MeshComponent>().each([this](MeshComponent& mesh_component) {
            mesh_component.mesh.destroy();
        });
    }
}