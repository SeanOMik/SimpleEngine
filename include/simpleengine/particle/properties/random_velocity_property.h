//
// Created by SeanOMik on 3/22/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANDOM_VELOCITY_PROPERTY_H
#define SIMPLEENGINE_RANDOM_VELOCITY_PROPERTY_H

#include "../particle_property.h"
#include "../../range_2.h"
#include "../../random.h"
#include "../particle.h"

#include <random>
#include <iostream>

namespace simpleengine {
    namespace particle {
        class RandomVelocityParticleProperty : public ParticleProperty {
        public:
            RandomVelocityParticleProperty(const simpleengine::Range2f& range) : range(range) {

            }

            RandomVelocityParticleProperty(float min_x, float max_x, float min_y, float max_y)
                : RandomVelocityParticleProperty(Range2f(min_x, max_x, min_y, max_y)) {

            }

            RandomVelocityParticleProperty(const RandomVelocityParticleProperty& other) {
                this->range = other.range;
            }

            std::unique_ptr<ParticleProperty> Clone() const override {
                return std::make_unique<RandomVelocityParticleProperty>(range);
            }

            void OnParticleSpawn(simpleengine::particle::Particle& particle) override {
                simpleengine::Random<float, std::random_device, std::mt19937_64> rand;
                sf::Vector2f velocity;
                velocity.x = rand.NextInRange<std::uniform_real_distribution<float>>(range.min_x, range.max_x);
                velocity.y = rand.NextInRange<std::uniform_real_distribution<float>>(range.min_y, range.max_y);

                particle.velocity = velocity;
            }

            void Update(simpleengine::particle::Particle& particle) override {
                
            }
        private:
            simpleengine::Range2f range;
        };
    }
}

#endif //SIMPLEENGINE_RANDOM_VELOCITY_PROPERTY_H
