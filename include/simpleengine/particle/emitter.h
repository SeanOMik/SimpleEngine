//
// Created by SeanOMik on 3/21/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_PARTICLE_EMITTER_H
#define SIMPLEENGINE_PARTICLE_EMITTER_H

#include "../event.h"
#include "particle_property.h"
#include "particle.h"
#include "../random.h"
#include "../range_2.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace simpleengine {
    namespace particle {
        struct ParticleAttributes {
            uint32_t lifetime_ms;
            sf::Vector2f start_velocity;
            float rotation_velocity = 0;
            sf::Vector2f scale = sf::Vector2f(1, 1);

            /* ParticleAttributes(uint32_t lifetime_ms, sf::Vector2f start_velocity, float rotation_velocity) : lifetime_ms(lifetime_ms), start_velocity(start_velocity), rotation_velocity(rotation_velocity) {

            } */
        };

        class ParticleEmitter : public simpleengine::Event {
        protected:
            using ParticlePropertyPtr = std::shared_ptr<simpleengine::particle::ParticleProperty>;
            using ParticlePropertyVector = std::vector<ParticlePropertyPtr>;
        public:
            explicit ParticleEmitter(sf::Texture& texture, sf::Vector2f position, double emit_variance, uint32_t emit_count, uint32_t particle_count, Range2f particle_range, ParticleAttributes attributes, ParticlePropertyVector& properties) 
                    : texture(texture), position(position), emit_variance(emit_variance), emit_count(emit_count), particle_count(particle_count), particle_range(particle_range), attributes(attributes), properties(properties) {
                time.restart();
            }

            void SetProperties(ParticlePropertyVector& properties) {
                this->properties = properties;
            }

            virtual void Update(const float& delta_time) override {
                for (std::vector<simpleengine::particle::Particle>::iterator it = particles.begin(); it != particles.end(); ) {
                    it->Update(delta_time);

                    if (it->IsDestroying()) {
                        it = particles.erase(it);
                    } else {
                        it++;
                    }
                }

                if (particles.size() < particle_count) {
                    if (random.NextInRange(0, 1) < emit_variance) {
                        for (uint32_t i = 0; i < emit_count; i++) {
                            Particle particle(texture, attributes.start_velocity, attributes.lifetime_ms, attributes.rotation_velocity, properties);

                            // Offset the position of the particle randomly
                            sf::Vector2f new_pos;
                            new_pos.x = random.NextInRange(particle_range.min_x, particle_range.max_x);
                            new_pos.y = random.NextInRange(particle_range.min_y, particle_range.max_y);
                            new_pos += position;
                            particle.GetSprite().setPosition(new_pos);
                    
                            particle.GetSprite().setScale(attributes.scale.x, attributes.scale.y);

                            particles.emplace_back(std::move(particle));
                        }
                    }
                }
            }

            virtual void Render(sf::RenderTarget* target) override {
                for (simpleengine::particle::Particle particle : particles) {
                    particle.Render(target);
                }
            }
        protected:
            sf::Texture& texture;
            sf::Vector2f position;
            double emit_variance;
            uint32_t emit_count;
            uint32_t particle_count;
            Range2f particle_range;
            ParticleAttributes attributes;
            ParticlePropertyVector& properties;

            std::vector<simpleengine::particle::Particle> particles;
            sf::Clock time;
            simpleengine::Random<double> random;
        };
    }
}

#endif //SIMPLEENGINE_PARTICLE_EMITTER_H
