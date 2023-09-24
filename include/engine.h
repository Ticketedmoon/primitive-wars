#pragma once

#ifndef PRIMITIVE_WARS_ENGINE_H
#define PRIMITIVE_WARS_ENGINE_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
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
        static void spawnPlayer();
        static void spawnEnemy();
        static void spawnBullet(sf::Vector2f position);
        static bool isCollidingAABB(
                const std::shared_ptr<CRender>& renderComponentForEntity,
                const std::shared_ptr<CRender>& renderComponentForEnemy);

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
        static inline uint32_t frameNo;

};

#endif //PRIMITIVE_WARS_ENGINE_H