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

namespace simpleengine {
    namespace particle {
        class RandomVelocityParticleProperty : public ParticleProperty {
        public:
            RandomVelocityParticleProperty(simpleengine::Range2f range) : range(range) {

            }

            void OnParticleSpawn(simpleengine::particle::Particle& particle) override {
                simpleengine::Random<float> rand;
                sf::Vector2f velocity;
                velocity.x = rand.NextInRange(range.min_x, range.max_x);
                velocity.y = rand.NextInRange(range.min_y, range.max_y);

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
