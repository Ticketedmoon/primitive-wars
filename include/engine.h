#pragma once

#ifndef PRIMITIVE_WARS_ENGINE_H
#define PRIMITIVE_WARS_ENGINE_H

#include <string_view>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <memory>

static constexpr std::string_view WINDOW_TITLE = "primitive-wars";
static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;

class Engine
{
    public:
        Engine();
        static void startGameLoop();

    private:
        // Game loop logic
        static void listenForEvents();
        static void update();
        static void render();

    private:
        static inline void createGameWindow();

    private:
        static inline sf::RenderWindow m_window;

};

#endif //PRIMITIVE_WARS_ENGINE_H