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

#include <chrono>
#include <memory>

namespace simpleengine {
    namespace particle {
        class Particle : public simpleengine::Event {
        private:
            using Super = simpleengine::Event;
        protected:
            using ParticlePropertyPtr = std::unique_ptr<simpleengine::particle::ParticleProperty>;
            using ParticlePropertyVector = std::vector<ParticlePropertyPtr>;
        public:
            friend class ParticleEmitter;

            template<class DurRep, class DurPeriod>
            Particle(sf::Texture& texture, sf::Vector2f velocity, std::chrono::duration<DurRep, DurPeriod> lifetime, float rotation_velocity,
                    ParticlePropertyVector properties)
                    : Super(nullptr), velocity(velocity), rotation_velocity(rotation_velocity), properties(std::move(properties)) {
                sprite.setTexture(texture);

                birth_point = std::chrono::high_resolution_clock::now();
                death_point = birth_point + lifetime;

                // Trigger OnParticleSpawn for particle properities.
                for (ParticlePropertyPtr& property : this->properties) {
                    property->OnParticleSpawn(*this);
                }
            }

            Particle(const Particle& other);
            Particle& operator=(Particle other);

            virtual void Update(const float& delta_time) override;
            virtual void Render(sf::RenderTarget* target) override;

            template<class DurRep, class DurPeriod>
            void SetLifetime(std::chrono::duration<DurRep, DurPeriod> lifetime) {
                death_point = std::chrono::high_resolution_clock::now() + lifetime;
            }

            std::chrono::microseconds GetAge() const;
            std::chrono::duration<long long, std::ratio<1, 1000000000>> GetLifetime() const;

            sf::Sprite sprite;
            sf::Vector2f velocity;
            float rotation_velocity;
            ParticlePropertyVector properties;
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> birth_point;
            std::chrono::time_point<std::chrono::high_resolution_clock> death_point;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_H
