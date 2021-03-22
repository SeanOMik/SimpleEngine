//
// Created by SeanOMik on 3/22/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANDOM_LIFETIME_PROPERTY_H
#define SIMPLEENGINE_RANDOM_LIFETIME_PROPERTY_H

#include "../particle_property.h"
#include "../../range.h"
#include "../../random.h"
#include "../particle.h"

namespace simpleengine {
    namespace particle {
        class RandomLifetimeParticleProperty : public ParticleProperty {
        public:
            RandomLifetimeParticleProperty(simpleengine::Rangef range) : range(range) {

            }

            void OnParticleSpawn(simpleengine::particle::Particle& particle) override {
                simpleengine::Random<float> rand;
                particle.lifetime_ms = rand.NextInRange(range.min, range.max);
            }

            void Update(simpleengine::particle::Particle& particle) override {
                
            }
        private:
            simpleengine::Rangef range;
        };
    }
}

#endif //SIMPLEENGINE_RANDOM_LIFETIME_PROPERTY_H
