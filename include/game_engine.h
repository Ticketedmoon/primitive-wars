#pragma once

#ifndef PRIMITIVE_WARS_GAME_ENGINE_H
#define PRIMITIVE_WARS_GAME_ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <string_view>
#include <memory>
#include <iostream>

#include "core/game_properties.h"
#include "scene.h"
#include "scene/gameplay_scene/gameplay_scene.h"
#include "scene/menu_scene/menu_scene.h"

static constexpr std::string_view WINDOW_TITLE = "primitive-wars";
static const bool USE_VERTICAL_SYNC = true;
static const uint32_t APP_FRAME_RATE = 60;

class GameEngine
{
    public:
        GameEngine();
        void startGameLoop();
        void changeScene(Scene::Type sceneType, const std::shared_ptr<Scene>& scene);

    private:
        void update();
        void render();

        void createGameWindow();


    public:
        sf::RenderWindow window;
        sf::Clock worldClock;
        sf::Clock deltaClock;
        GameProperties gameProperties{false, 0, 0, worldClock.getElapsedTime().asSeconds(), worldClock.getElapsedTime().asSeconds()};

    private:
        Scene::Type currentScene = Scene::Type::MENU_SCENE;
        std::unordered_map<Scene::Type, std::shared_ptr<Scene>> gameScenes;
};

#endif //PRIMITIVE_WARS_GAME_ENGINE_H