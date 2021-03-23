#include "particle/emitter.h"

simpleengine::particle::ParticleEmitter::ParticleEmitter(sf::Texture& texture, sf::Vector2f position, double emit_variance, uint32_t emit_count, uint32_t particle_count, Range2f particle_range, ParticleAttributes attributes, ParticlePropertyVector& properties) : texture(texture), position(position), emit_variance(emit_variance), emit_count(emit_count), particle_count(particle_count), particle_range(particle_range), attributes(attributes), properties(properties) {
    time.restart();
}

void simpleengine::particle::ParticleEmitter::SetProperties(ParticlePropertyVector& properties) {
    this->properties = properties;
}

void simpleengine::particle::ParticleEmitter::Update(const float &delta_time) {
    // Update particles and check if they are being destroyed. If they are, destroy them.
    for (std::vector<simpleengine::particle::Particle>::iterator it = particles.begin(); it != particles.end(); ) {
        it->Update(delta_time);

        if (it->IsDestroying()) {
            it = particles.erase(it);
        } else {
            it++;
        }
    }

    // Check if we can spawn particles with max particle count, also use chance to check if we can.
    if (particles.size() < particle_count && random.NextInRange(0, 1) < emit_variance) {
        // Make sure that we don't emit too many particles to go over the max particle count
        uint32_t emitting_ct = (particles.size() + emit_count > particle_count) ? particle_count - particles.size() : emit_count;
        for (uint32_t i = 0; i < emitting_ct; i++) {
            Particle particle(texture, attributes.start_velocity, attributes.lifetime_ms, attributes.rotation_velocity, properties);

            // Offset the position of the particle randomly
            sf::Vector2f new_pos;
            new_pos.x = random.NextInRange(particle_range.min_x, particle_range.max_x);
            new_pos.y = random.NextInRange(particle_range.min_y, particle_range.max_y);
            new_pos += position;
            particle.GetSprite().setPosition(new_pos);
    
            particle.GetSprite().setScale(attributes.scale.x, attributes.scale.y);

            particles.emplace_back(std::move(particle));
        }
    }
}

void simpleengine::particle::ParticleEmitter::Render(sf::RenderTarget *target) {
    for (simpleengine::particle::Particle particle : particles) {
        particle.Render(target);
    }
}