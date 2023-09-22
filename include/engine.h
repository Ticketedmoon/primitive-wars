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

#include "entity.h"
#include "entity_manager.h"
#include "c_transform.h"
#include "c_user_input.h"
#include "c_collision.h"
#include "c_render.h"

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
        void render(sf::Sprite& backgroundSprite);

    private:
        static inline void createGameWindow();
        static void createPlayer();

        static void transformSystem();
        static void collisionSystem();
        static void userInputSystem();
        static void renderSystem();

    private:
        static inline sf::RenderWindow m_window;
        static inline EntityManager m_entityManager;

};

#endif //PRIMITIVE_WARS_ENGINE_H