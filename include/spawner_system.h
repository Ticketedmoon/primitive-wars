#pragma once

#ifndef PRIMITIVE_WARS_SPAWNER_SYSTEM_H
#define PRIMITIVE_WARS_SPAWNER_SYSTEM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <utility>
#include <experimental/random>

#include "c_transform.h"
#include "entity.h"
#include "entity_manager.h"
#include "c_collision.h"
#include "c_user_input.h"
#include "c_render.h"
#include "c_lifespan.h"

struct SpawnProperties
{
    size_t totalVertices{};
    Entity::Type entityType = Entity::Type::NONE;
    bool isCollidable{};
    sf::Vector2f speed;
};

class EntitySpawner
{
    public:
        EntitySpawner(EntityManager& entityManager, uint16_t windowWidth, uint16_t windowHeight);

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
        EntityManager& m_entityManager;
        uint16_t m_windowWidth;
        uint16_t m_windowHeight;

        float PI_FULL_CIRCLE = std::numbers::pi_v<float> * 2;
};


#endif //PRIMITIVE_WARS_SPAWNER_SYSTEM_H
