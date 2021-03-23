//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_PARTICLE_H
#define SIMPLEENGINE_PARTICLE_H

#include "../event.h"
#include "particle_property.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>

namespace simpleengine {
    namespace particle {
        class Particle : public simpleengine::Event {
        private:
            using Super = simpleengine::Event;
        protected:
            using ParticlePropertyPtr = std::shared_ptr<simpleengine::particle::ParticleProperty>;
            using ParticlePropertyVector = std::vector<ParticlePropertyPtr>;
        public:
            Particle(sf::Texture& texture, sf::Vector2f velocity, uint32_t lifetime_ms, float rotation_velocity, ParticlePropertyVector properties);

            virtual void Update(const float& delta_time) override;
            virtual void Render(sf::RenderTarget* target) override;

            sf::Sprite& GetSprite();
            const sf::Time GetAge() const;

            sf::Sprite sprite;
            sf::Vector2f velocity;
            uint32_t lifetime_ms;
            float rotation_velocity;
            ParticlePropertyVector properties;
            
        private:
            sf::Clock age_clock;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_H
