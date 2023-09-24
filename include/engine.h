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

#include <experimental/random>
#include <cmath>

#include "entity.h"
#include "entity_manager.h"
#include "c_transform.h"
#include "c_user_input.h"
#include "c_collision.h"
#include "c_render.h"
#include "c_lifespan.h"

static constexpr std::string_view WINDOW_TITLE = "primitive-wars";
static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;
static const std::string FONT_PATH = "resources/fonts/calibri.ttf";

static const sf::String& PAUSED_TEXT = "GAME PAUSED";
static const sf::Vector2<float>& PAUSED_TEXT_POSITION = sf::Vector2f(WINDOW_WIDTH / 2 - 12, WINDOW_HEIGHT / 2 - 48);

class Engine
{
    public:
        Engine();
        void startGameLoop();

    private:
        // Game loop logic
        static void listenForEvents();
        static void update();
        void render();

    private:
        static inline void createGameWindow();
        void configureTextRendering();

        static void spawnPlayer();
        static void spawnEnemy();
        static void spawnBullet(sf::Vector2f position);
        static bool isCollidingAABB(
                const std::shared_ptr<CRender>& renderComponentForEntity,
                const std::shared_ptr<CRender>& renderComponentForEnemy);
        static void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize,
                sf::Vector2f position);

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
        static inline sf::Font m_font;
        static inline sf::Text gameOverlayText;
        static inline sf::Text pauseText;
        static inline bool hasPaused;

        static void checkForWindowCollision(const std::shared_ptr<Entity>& e);
};

#endif //PRIMITIVE_WARS_ENGINE_H