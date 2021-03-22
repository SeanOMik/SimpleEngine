//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_PARTICLE_PROPERTY_H
#define SIMPLEENGINE_PARTICLE_PROPERTY_H

#include "../event.h"

#include <SFML/Graphics/RenderStates.hpp>

namespace simpleengine {
    namespace particle {
        class Particle;
        class ParticleProperty {
        public:
            virtual void OnParticleSpawn(simpleengine::particle::Particle& particle) = 0;
            virtual void Update(simpleengine::particle::Particle& particle) = 0;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_PROPERTY_H
