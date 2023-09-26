#pragma once

#ifndef PRIMITIVE_WARS_ENGINE_H
#define PRIMITIVE_WARS_ENGINE_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <string_view>
#include <ranges>
#include <iostream>
#include <memory>
#include <numeric>
#include <cassert>

#include <experimental/random>
#include <cmath>

#include "entity.h"
#include "entity_manager.h"
#include "component/c_transform.h"
#include "component/c_user_input.h"
#include "component/c_collision.h"
#include "component/c_render.h"
#include "component/c_lifespan.h"

static constexpr std::string_view WINDOW_TITLE = "primitive-wars";
static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;
static const std::string BACKGROUND_IMAGE_PATH = "resources/assets/board.png";
static const std::string FONT_PATH = "resources/fonts/calibri.ttf";

static const sf::String& PAUSED_TEXT = "GAME PAUSED";
static const sf::Vector2<float>& CENTRE_SCREEN_POSITION = sf::Vector2f(WINDOW_WIDTH / 2 - 496, WINDOW_HEIGHT / 2 - 96);
static const uint8_t SPECIAL_ATTACK_COOLDOWN_OFFSET = 5;
static const uint8_t PLAYER_DEAD_TIME_OFFSET = 3;

class Engine
{
    public:
        Engine();
        static void startGameLoop();

    private:
        // Game loop logic
        static void update();
        static void render();

    private:
        struct SpawnProperties
        {
            Entity::Type entityType;
            double shotAngle;
            bool isCollidable;
            sf::Vector2f speed;
        };

        struct ShapeProperties
        {
            float radius;
            size_t totalVertices;
            sf::Vector2f position;
            sf::Color fillColor;
            sf::Color outlineColor;
            float outlineThickness;
        };

        static inline void createGameWindow();
        static void configureTextRendering();

        static void spawnPlayer();
        static void spawnEnemy();
        static void spawnBullet(sf::Vector2f position, double shotAngle);
        static void spawnEntityClone(const std::shared_ptr<Entity>& existingEnemy, SpawnProperties spawnProperties);

        static bool isCollidingAABB(
                const std::shared_ptr<CRender>& renderComponentForEntity,
                const std::shared_ptr<CRender>& renderComponentForEnemy);
        static void checkForWindowCollision(const std::shared_ptr<Entity>& e);
        static bool isNearPlayer(sf::FloatRect enemyBoundingBox);

        static void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize,
                sf::Vector2f position);
        static sf::CircleShape createShape(ShapeProperties properties);

        static void transformSystem();
        static void collisionSystem();
        static void enemySpawnSystem();
        static void userInputSystem();
        static void lifeSpanSystem();
        static void renderSystem();

    private:

        static inline sf::RenderWindow m_window;
        static inline EntityManager m_entityManager;
        static inline sf::Texture textureSprite;
        static inline sf::Sprite backgroundSprite;
        static inline size_t frameNo;
        static inline size_t score = 0;
        static inline size_t totalDeaths;

        static inline sf::Clock worldClock;
        static inline float playerDeadTimer = worldClock.getElapsedTime().asSeconds();
        static inline float specialAttackCoolDown = worldClock.getElapsedTime().asSeconds();

        static inline sf::Text gameOverlayText;
        static inline sf::Text respawnText;
        static inline sf::Text pauseText;
        static inline bool hasPaused;
        static inline sf::Font m_font;
};

#endif //PRIMITIVE_WARS_ENGINE_H