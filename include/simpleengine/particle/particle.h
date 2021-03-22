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
            using super = simpleengine::Event;
        protected:
            using ParticlePropertyPtr = std::shared_ptr<simpleengine::particle::ParticleProperty>;
            using ParticlePropertyVector = std::vector<ParticlePropertyPtr>;
        public:
            Particle(sf::Texture& texture, sf::Vector2f velocity, uint32_t lifetime_ms, float rotation_velocity,
                    ParticlePropertyVector properties) : simpleengine::Event(nullptr), velocity(velocity), lifetime_ms(lifetime_ms),
                    rotation_velocity(rotation_velocity), properties(properties) {
                age_clock.restart();
                sprite.setTexture(texture);

                for (ParticlePropertyPtr property : properties) {
                    property->OnParticleSpawn(*this);
                }
            }

            virtual void Update(const float& delta_time) override {
                if (age_clock.getElapsedTime().asMilliseconds() >= lifetime_ms) {
                    super::Destroy();
                    return;
                }

                for (ParticlePropertyPtr property : properties) {
                    property->Update(*this);
                }

                sprite.move(velocity.x, velocity.y);
            }

            virtual void Render(sf::RenderTarget* target) override {
                target->draw(sprite);
            }

            sf::Sprite& GetSprite() {
                return sprite;
            }

            /* void SetVelocity(const sf::Vector2f& velocity) {
                this->velocity = velocity;
            }

            const sf::Vector2f& GetVelocity() {
                return velocity;
            }

            void SetRotationVelocity(const float& rotation_velocity) {
                this->rotation_velocity = rotation_velocity;
            }

            const float& GetRotationVelocity() {
                return rotation_velocity;
            }
        private: */

            const sf::Time GetAge() const {
                return age_clock.getElapsedTime();
            }

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
