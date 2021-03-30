//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "simpleengine/random.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <memory>
#include <ratio>
#include <simpleengine/game.h>
#include <simpleengine/event.h>
#include <simpleengine/entity.h>
#include <simpleengine/range.h>

// Particle
#include <simpleengine/particle/emitter.h>
#include <simpleengine/particle/particle_property.h>
#include <simpleengine/particle/particle_emitter_property.h>
#include <simpleengine/particle/properties/random_lifetime_property.h>
#include <simpleengine/particle/properties/random_velocity_property.h>

#include <chrono>
#include <iostream>
#include <iomanip>
#include <ios>
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

namespace se = simpleengine;

class ParticleEmitter : public se::particle::ParticleEmitter {
private:
    using Super = se::particle::ParticleEmitter;

    sf::Sprite sprite;
    sf::Texture texture;

    Super::ParticlePropertyVector particle_properties;
    Super::EmitterPropertyVector emitter_properties;
public:
    ParticleEmitter() : Super(texture, sf::Vector2f(350, 350), .5f, 5, 900, se::Range2f(-25, 25, -25, 25),
            se::particle::ParticleAttributes{std::chrono::milliseconds(500), sf::Vector2f(.1f, .1f), 5,
            sf::Vector2f(2, 2)}, particle_properties, emitter_properties) {
        texture.loadFromFile("particle.png"); // The particle I tested with was 5x5 pixels
        texture.setSmooth(true);

        particle_properties.emplace_back(std::make_unique<se::particle::RandomVelocityParticleProperty>(se::Range2f(-1.5f, 1.5f, -1.5f, 1.5f)));
        
        using namespace std::chrono_literals;
        particle_properties.emplace_back(std::make_unique<se::particle::RandomLifetimeParticleProperty>(1'500ms, 2'500ms));
    }

    void Update(const float& delta_time) override {
        Super::Update(delta_time);
    }

    void Render(sf::RenderTarget* target) override {
        Super::Render(target);
    }

    void SetPosition(int x, int y) {
        position.x = x;
        position.y = y;
    }
};

void OnMouseMove(ParticleEmitter* emitter, sf::Event::MouseMoveEvent event) {
    emitter->SetPosition(event.x, event.y);
}

int main(int argc, char *argv[]) {
    simpleengine::Game game(700, 700, "SimpleEngine - Particles Example");

    ParticleEmitter* emitter = new ParticleEmitter();
    game.AddEventCallback<sf::Event::EventType::MouseMoved>(std::function([&](sf::Event::MouseMoveEvent event) {
        OnMouseMove(emitter, event);
    }));

    game.AddEvent(emitter);

    return game.Run();
}