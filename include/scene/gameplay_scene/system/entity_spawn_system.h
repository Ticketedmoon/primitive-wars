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
#include "core/common_constants.h"
#include "entity/entity.h"
#include "entity/entity_manager.h"
#include "c_collision.h"
#include "c_action.h"
#include "c_render.h"
#include "c_lifespan.h"
#include "system.h"
#include "gui_updater_system.h"

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
        explicit EntitySpawnSystem(EntityManager& entityManager, GameProperties& gameProperties);

        void execute(GameProperties& gameProperties) override;
        bool shouldApply(GameProperties gameProperties) override;

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

        static inline const sf::Vector2f PLAYER_SPEED{300.0f, 300.0f};
        static inline const sf::Vector2f SPECIAL_ATTACK_SPEED{400.0f, 400.0f};
        static constexpr int ENEMY_SPEED_LOWER_BOUND = 50;
        static constexpr int ENEMY_SPEED_UPPER_BOUND = 150;
        static inline const sf::Vector2f ENEMY_DEATH_ANIMATION_SPEED{100.0f, 100.0f};

        static const uint8_t SPECIAL_ATTACK_COOLDOWN_OFFSET = 3;
        static constexpr uint8_t ENEMY_SCORE_MULTIPLIER = 100;
        static constexpr uint16_t ENEMY_SPAWN_OFFSET = 512;

        EntityManager& m_entityManager;
        GameProperties& m_gameProperties;

        float enemyRespawnTimeSeconds{
            m_gameProperties.getLevelClock().getElapsedTime().asSeconds() + m_gameProperties.getEnemySpawnRateSeconds()};
};


#endif //PRIMITIVE_WARS_ENTITY_SPAWN_SYSTEM_H
