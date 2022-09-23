#include "scene.h"
#include "ecs/component/mesh_component.h"
#include "ecs/component/model_component.h"
#include "ecs/component/transform_component.h"
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
                renderer->queue_job(gfx::RenderingJob(mesh, transform.transform_matrix));
            }
        });

        registry.view<const TransformComponent, MeshComponent>().each([this](const TransformComponent& transform, MeshComponent& mesh_component) {
            renderer->queue_job(gfx::RenderingJob(mesh_component.mesh, transform.transform_matrix));
        });
    }
}