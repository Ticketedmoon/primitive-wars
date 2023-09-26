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
        EntitySpawnSystem(EntityManager& entityManager, sf::Clock& worldClock, size_t& frameNo,
                float& playerRespawnTimeSeconds);

        void execute() override;

        // FIXME these should be private
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

        void spawnPlayer();
        void spawnEnemy();
        void spawnEntityClone(const std::shared_ptr<Entity>& existingEntity, const SpawnProperties& spawnProperties,
                double shotAngle);
        static sf::CircleShape createShape(ShapeProperties properties);
        bool isNearPlayer(sf::FloatRect enemyBoundingBox);

    private:
        EntityManager& m_entityManager;
        sf::Clock& m_worldClock;
        size_t& m_frameNo;
        float& m_playerRespawnTimeSeconds;

        static constexpr float PI_FULL_CIRCLE = std::numbers::pi_v<float> * 2;
};


#endif //PRIMITIVE_WARS_ENTITY_SPAWN_SYSTEM_H
