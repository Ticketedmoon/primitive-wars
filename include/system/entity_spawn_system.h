#pragma once

#ifndef PRIMITIVE_WARS_ENTITY_SPAWN_SYSTEM_H
#define PRIMITIVE_WARS_ENTITY_SPAWN_SYSTEM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <utility>
#include <experimental/random>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#include "c_transform.h"
#include "common_constants.h"
#include "entity.h"
#include "entity_manager.h"
#include "c_collision.h"
#include "c_user_input.h"
#include "c_render.h"
#include "c_lifespan.h"
#include "system.h"
#include "gui_system.h"

struct SpawnProperties
{
    size_t totalVertices{};
    Entity::Type entityType = Entity::Type::NONE;
    bool isCollidable{};
    sf::Vector2f speed;
};

class EntitySpawnSystem : public System
{
    public:
        explicit EntitySpawnSystem(EntityManager& entityManager, sf::Clock& worldClock, GuiProperties& guiProperties);

        void execute() override;

        // FIXME investigate moving these to be private
        void spawnPlayer();
        void spawnEnemy();
        void spawnBullet(sf::Vector2f position, double shotAngle);
        void spawnEntityAnimation(const std::shared_ptr<Entity>& existingEntity, const SpawnProperties& spawnProperties);

    private:
        struct ShapeProperties
        {
            float radius{};
            size_t totalVertices{};
            sf::Vector2f position;
            sf::Color fillColor;
            sf::Color outlineColor;
            float outlineThickness{};
        };

        void spawnEntityClone(const std::shared_ptr<Entity>& existingEntity, const SpawnProperties& spawnProperties,
                double shotAngle);
        static sf::CircleShape createShape(ShapeProperties properties);
        bool isNearPlayer(sf::FloatRect enemyBoundingBox);

    private:
        static constexpr float PI_FULL_CIRCLE = std::numbers::pi_v<float> * 2;
        static constexpr float ENEMY_SPAWN_RATE_SECONDS = 1.0f;
        static const uint8_t SPECIAL_ATTACK_COOLDOWN_OFFSET = 3;

        EntityManager& m_entityManager;
        sf::Clock& m_worldClock;
        GuiProperties& m_guiProperties;

        float enemyRespawnTimeSeconds{m_worldClock.getElapsedTime().asSeconds() + ENEMY_SPAWN_RATE_SECONDS};
};


#endif //PRIMITIVE_WARS_ENTITY_SPAWN_SYSTEM_H
