#include "scene.h"
#include "ecs/component/mesh_component.h"
#include "ecs/component/model_component.h"
#include "ecs/component/transform_component.h"
#include "ecs/component/rotating_component.h"
#include "ecs/entity.h"
#include "gfx/renderer.h"

namespace simpleengine {
    Scene::Scene(std::shared_ptr<gfx::Renderer> renderer) : renderer(renderer) {
        
    }

    ecs::Entity Scene::create_entity() {
        return ecs::Entity(registry, registry.create());
    }

    void Scene::update(const float& delta_time) {
        // Is there a way these can be grouped?
        registry.view<const TransformComponent, ModelComponent>().each([this](const TransformComponent& transform, ModelComponent& model_component) {
            for (auto& mesh : model_component.model.meshes) {
                auto rendering_type = gfx::RenderingType::RendType_OPAQUE;
                if (mesh.material) {
                    rendering_type = mesh.material->rendering_type;
                }

                renderer->queue_job(gfx::RenderingJob(rendering_type, mesh, transform.transform_matrix));
            }
        });

        registry.view<const TransformComponent, MeshComponent>().each([this](const TransformComponent& transform, MeshComponent& mesh_component) {
            auto rendering_type = gfx::RenderingType::RendType_OPAQUE;
            if (mesh_component.mesh.material) {
                rendering_type = mesh_component.mesh.material->rendering_type;
            }

            renderer->queue_job(gfx::RenderingJob(rendering_type, mesh_component.mesh, transform.transform_matrix));
        });

        registry.view<TransformComponent, RotatingComponent>().each([this, &delta_time](TransformComponent& transform, RotatingComponent& rotating) {
            transform.rotate(rotating.rate * delta_time, rotating.rotation_axis);
        });
    }
}