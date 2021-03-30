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

#include <chrono>
#include <memory>
#include <iostream>
#include <ratio>

namespace simpleengine {
    namespace particle {
        class RandomLifetimeParticleProperty : public ParticleProperty {
        public:
            template<class DurRep, class DurPeriod>
            RandomLifetimeParticleProperty(std::chrono::duration<DurRep, DurPeriod> min, std::chrono::duration<DurRep, DurPeriod> max)
                : range(Range<std::chrono::milliseconds>(
                    std::chrono::duration_cast<std::chrono::milliseconds>(min),
                    std::chrono::duration_cast<std::chrono::milliseconds>(max))) {

            }

            template<class DurRep, class DurPeriod>
            RandomLifetimeParticleProperty(const simpleengine::Range<std::chrono::duration<DurRep, DurPeriod>>& range) : RandomLifetimeParticleProperty(range.min, range.max) {

            }

            RandomLifetimeParticleProperty(const RandomLifetimeParticleProperty& other) {
                this->range = other.range;
            }

            std::unique_ptr<ParticleProperty> Clone() const override {
                return std::make_unique<RandomLifetimeParticleProperty>(range);
            }

            void OnParticleSpawn(simpleengine::particle::Particle& particle) override {
                simpleengine::Random<double, std::random_device, std::mt19937_64> rand;

                int lifetime = rand.NextInRange(range.min.count(), range.max.count());
                particle.SetLifetime(std::chrono::milliseconds(lifetime));

                //std::cout << "Lifetime: " << lifetime << "ms" << std::endl;
            }

            void Update(simpleengine::particle::Particle& particle) override {
                
            }
        private:
            simpleengine::Range<std::chrono::milliseconds> range;
        };
    }
}

#endif //SIMPLEENGINE_RANDOM_LIFETIME_PROPERTY_H
