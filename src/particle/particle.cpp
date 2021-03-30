#include "particle/particle.h"
#include "particle/particle_property.h"

#include <chrono>
#include <iostream>

simpleengine::particle::Particle::Particle(const simpleengine::particle::Particle& other) {
    // From simpleengine::Destructable
    this->destroying = other.destroying;

    this->sprite = other.sprite;
    this->velocity = other.velocity;
    this->rotation_velocity = other.rotation_velocity;
    
    // Copy the properties from the other particle. Make sure to empty properties.
    this->properties.clear();
    for(const ParticlePropertyPtr& property : other.properties) {
        std::unique_ptr<ParticleProperty> ptr = property->Clone();
        this->properties.push_back(std::move(ptr));
    }

    birth_point = other.birth_point;
    death_point = other.death_point;
}

simpleengine::particle::Particle& simpleengine::particle::Particle::operator=(Particle other) {
    // From simpleengine::Destructable
    this->destroying = other.destroying;

    this->sprite = other.sprite;
    this->velocity = other.velocity;
    this->rotation_velocity = other.rotation_velocity;
    
    // Copy the properties from the other particle. Make sure to empty properties.
    this->properties.clear();
    for(const ParticlePropertyPtr& property : other.properties) {
        std::unique_ptr<ParticleProperty> ptr = property->Clone();
        this->properties.push_back(std::move(ptr));
    }

    birth_point = other.birth_point;
    death_point = other.death_point;

    return *this;
}

void simpleengine::particle::Particle::Update(const float& delta_time) {
    // If death_point has passed then destroy the particle.
    if (std::chrono::high_resolution_clock::now().time_since_epoch() > death_point.time_since_epoch()) {
        Super::Destroy();
        return;
    }

    // Update all properties.
    for (ParticlePropertyPtr& property : properties) {
        property->Update(*this);
    }

    sprite.move(velocity.x, velocity.y);
}

void simpleengine::particle::Particle::Render(sf::RenderTarget* target) {
    target->draw(sprite);
}

std::chrono::microseconds simpleengine::particle::Particle::GetAge() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - birth_point);
}

std::chrono::duration<long long, std::ratio<1, 1000000000>> simpleengine::particle::Particle::GetLifetime() const {
    return death_point - birth_point;
}