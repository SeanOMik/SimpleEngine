//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector3.hpp>

#include <simpleengine/game.h>
#include <simpleengine/event.h>
#include <simpleengine/entity.h>
#include <simpleengine/range.h>

// Particle
#include <simpleengine/particle/emitter.h>
#include <simpleengine/particle/particle_property.h>
#include <simpleengine/particle/properties/random_lifetime_property.h>
#include <simpleengine/particle/properties/random_velocity_property.h>

#include <chrono>
#include <iostream>
#include <iomanip>
#include <ios>
#include <random>

namespace se = simpleengine;

class ParticleEmitter : public se::particle::ParticleEmitter {
private:
    using super = se::particle::ParticleEmitter;

    sf::Sprite sprite;
    sf::Texture texture;

    super::ParticlePropertyVector properties = {
        std::make_shared<se::particle::RandomVelocityParticleProperty>(se::Range2f(-1.5f, 1.5f, -1.5f, 1.5f)),
        std::make_shared<se::particle::RandomLifetimeParticleProperty>(se::Rangef(1'500, 9'500))
    };
public:
    ParticleEmitter() : super(texture, sf::Vector2f(350, 350), 0.4, 5, 1500, se::Range2f(-15, 15, -15, -15), se::particle::ParticleAttributes{0, sf::Vector2f(), 5, sf::Vector2f(2, 2)}, properties) {
        texture.loadFromFile("particle.png"); // The particle I tested with was 5x5 pixels
        texture.setSmooth(true);
    }

    void Update(const float& delta_time) override {
        super::Update(delta_time);
    }

    void Render(sf::RenderTarget* target) override {
        super::Render(target);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(700, 700, "SimpleEngine - Particles Example");
    game.AddEvent(new ParticleEmitter());

    return game.Run();
}