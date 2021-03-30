//
// Created by SeanOMik on 3/23/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_PARTICLE_EMITTER_PROPERTY_H
#define SIMPLEENGINE_PARTICLE_EMITTER_PROPERTY_H

#include "../event.h"

namespace simpleengine {
    namespace particle {
        class Particle;
        class ParticleEmitterProperty {
        public:
            //virtual void Clone() = 0;
            virtual void BeforeParticleSpawn(simpleengine::particle::Particle& particle) = 0;
            virtual void Update(simpleengine::particle::Particle& particle) = 0;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_EMITTER_PROPERTY_H
