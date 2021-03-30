#include "particle/emitter.h"
#include "destructable.h"

#include <chrono>
#include <iostream>

simpleengine::particle::ParticleEmitter::ParticleEmitter(sf::Texture& texture, sf::Vector2f position, double emit_variance,
        uint32_t emit_count, uint32_t particle_count, Range2f particle_range, ParticleAttributes attributes,
        ParticlePropertyVector& particle_properties, EmitterPropertyVector& emitter_properties)
        : texture(texture), position(position), emit_variance(emit_variance), emit_count(emit_count), particle_count(particle_count),
        particle_range(particle_range), attributes(attributes), particle_properties(particle_properties), emitter_properties(emitter_properties) {
    time.restart();
}

void simpleengine::particle::ParticleEmitter::SetProperties(ParticlePropertyVector particle_properties) {
    this->particle_properties = std::move(particle_properties);
}

void simpleengine::particle::ParticleEmitter::Update(const float &delta_time) {
    // Update each particle and remove it from the vector if it is ready to be destroyed.
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [&delta_time](simpleengine::particle::Particle& particle){
            particle.Update(delta_time);

            return particle.IsDestroying();
        }), particles.end());

    // Check if we can spawn particles with max particle count, also use chance to check if we can.
    if (particles.size() + emit_count <= particle_count && random.NextInRange(0, 1) < emit_variance) {
        // Make sure that we don't emit too many particles to go over the max particle count
        for (uint32_t i = 0; i < emit_count; i++) {

            ParticlePropertyVector particle_properties_clone;
            for (ParticlePropertyPtr& property : particle_properties) {
                particle_properties_clone.push_back(std::move(property->Clone()));
            }

            Particle particle(texture, attributes.start_velocity, attributes.lifetime, attributes.rotation_velocity, std::move(particle_properties_clone));

            // Offset the position of the particle randomly
            sf::Vector2f new_pos;
            new_pos.x = random.NextInRange(particle_range.min_x, particle_range.max_x);
            new_pos.y = random.NextInRange(particle_range.min_y, particle_range.max_y);
            new_pos += position;
            particle.sprite.setPosition(new_pos);
    
            particle.sprite.setScale(attributes.scale.x, attributes.scale.y);

            // Run the emitter properties.
            for(EmitterPropertyPtr& emitter_property : emitter_properties) {
                emitter_property->BeforeParticleSpawn(particle);
            }

            particles.emplace_back(std::move(particle));
        }
    }

    if (particles.size() > 0) {
        std::chrono::milliseconds avg_particle_lifetime(0);
        std::chrono::milliseconds max_lifetime(0);
        for (const Particle& particle : particles) {
            auto lifetime = std::chrono::duration_cast<std::chrono::milliseconds>(particle.death_point - particle.birth_point);
            avg_particle_lifetime += lifetime;
            if (lifetime > max_lifetime) {
                max_lifetime = lifetime;
            }
        }
        avg_particle_lifetime /= particles.size();
    }
}

void simpleengine::particle::ParticleEmitter::Render(sf::RenderTarget *target) {
    for (simpleengine::particle::Particle particle : particles) {
        particle.Render(target);
    }
}