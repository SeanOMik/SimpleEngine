//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_PARTICLE_EMITTER_H
#define SIMPLEENGINE_PARTICLE_EMITTER_H

#include "../event.h"
#include "particle_property.h"
#include "particle_emitter_property.h"
#include "particle.h"
#include "../random.h"
#include "../range_2.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include <SFML/System/Vector2.hpp>

namespace simpleengine {
    namespace particle {
        struct ParticleAttributes {
            uint32_t lifetime_ms;
            sf::Vector2f start_velocity;
            float rotation_velocity = 0;
            sf::Vector2f scale = sf::Vector2f(1, 1);
        };

        class ParticleEmitter : public simpleengine::Event {
        protected:
            using ParticlePropertyPtr = std::unique_ptr<simpleengine::particle::ParticleProperty>;
            using ParticlePropertyVector = std::vector<ParticlePropertyPtr>;

            using EmitterPropertyPtr = std::unique_ptr<simpleengine::particle::ParticleEmitterProperty>;
            using EmitterPropertyVector = std::vector<EmitterPropertyPtr>;
        public:
            ParticleEmitter(sf::Texture& texture, sf::Vector2f position, double emit_variance, uint32_t emit_count, uint32_t particle_count, Range2f particle_range, ParticleAttributes attributes, ParticlePropertyVector& particle_properties, EmitterPropertyVector& emitter_properties);

            void SetProperties(ParticlePropertyVector particle_properties);

            virtual void Update(const float& delta_time) override;
            virtual void Render(sf::RenderTarget* target) override;
        protected:
            sf::Texture& texture;
            sf::Vector2f position;
            double emit_variance;
            uint32_t emit_count;
            uint32_t particle_count;
            Range2f particle_range;
            ParticleAttributes attributes;
            ParticlePropertyVector& particle_properties;
            EmitterPropertyVector& emitter_properties;

            std::vector<simpleengine::particle::Particle> particles;
            sf::Clock time;
            simpleengine::Random<double> random;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_EMITTER_H
