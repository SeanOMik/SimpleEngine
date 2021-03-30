//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_PARTICLE_PROPERTY_H
#define SIMPLEENGINE_PARTICLE_PROPERTY_H

#include "../event.h"
#include <algorithm>

namespace simpleengine {
    namespace particle {
        class Particle;
        class ParticleProperty {
        public:
            virtual std::unique_ptr<ParticleProperty> Clone() const = 0;
            virtual void OnParticleSpawn(simpleengine::particle::Particle& particle) = 0;
            virtual void Update(simpleengine::particle::Particle& particle) = 0;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_PROPERTY_H
