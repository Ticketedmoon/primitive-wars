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
#include "entity_spawn_system.h"
#include "transform_system.h"
#include "collision_system.h"
#include "lifespan_system.h"
#include "render_system.h"
#include "gui_system.h"

static constexpr std::string_view WINDOW_TITLE = "primitive-wars";
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;
static const uint8_t SPECIAL_ATTACK_COOLDOWN_OFFSET = 3;

class Engine
{
    public:
        Engine();
        static void startGameLoop();

    private:
        static void update();
        static void render();

    private:
        static inline void createGameWindow();

        static void userInputSystem();

    private:
        static const inline std::string BACKGROUND_IMAGE_PATH = "resources/assets/board.png";

        static inline sf::RenderWindow m_window;
        static inline EntityManager m_entityManager;
        static inline sf::Clock worldClock;
        static inline sf::Clock deltaClock;

        static inline sf::Texture textureSprite;
        static inline sf::Sprite backgroundSprite;

        static inline GuiProperties guiProperties{false, 0, 0, worldClock.getElapsedTime().asSeconds(),
                                                  worldClock.getElapsedTime().asSeconds()};

        static inline TransformSystem m_transformSystem{m_entityManager};
        static inline EntitySpawnSystem m_entitySpawnerSystem{m_entityManager, worldClock, guiProperties};
        static inline CollisionSystem m_collisionSystem{m_entityManager};
        static inline LifespanSystem m_lifespanSystem{m_entityManager};
        static inline RenderSystem m_renderSystem{m_window, m_entityManager};
        static inline GuiSystem m_guiSystem{m_window, m_entityManager, worldClock, guiProperties};
};

#endif //PRIMITIVE_WARS_ENGINE_H