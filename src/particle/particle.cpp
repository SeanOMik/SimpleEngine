#include "particle/particle.h"

simpleengine::particle::Particle::Particle(sf::Texture& texture, sf::Vector2f velocity, uint32_t lifetime_ms, float rotation_velocity,
        ParticlePropertyVector properties) : Super(nullptr), velocity(velocity), lifetime_ms(lifetime_ms),
        rotation_velocity(rotation_velocity), properties(properties) {
    age_clock.restart(); // Start age clock
    sprite.setTexture(texture);

    // Trigger OnParticleSpawn for particle properities.
    for (ParticlePropertyPtr property : properties) {
        property->OnParticleSpawn(*this);
    }
}

void simpleengine::particle::Particle::Update(const float& delta_time) {
    // If the particle is older than its lifetime, destroy it.
    if (age_clock.getElapsedTime().asMilliseconds() >= lifetime_ms) {
        Super::Destroy();
        return;
    }

    // Update all properties.
    for (ParticlePropertyPtr property : properties) {
        property->Update(*this);
    }

    sprite.move(velocity.x, velocity.y);
}

void simpleengine::particle::Particle::Render(sf::RenderTarget* target) {
    target->draw(sprite);
}

sf::Sprite& simpleengine::particle::Particle::GetSprite() {
    return sprite;
}

const sf::Time simpleengine::particle::Particle::GetAge() const {
    return age_clock.getElapsedTime();
}