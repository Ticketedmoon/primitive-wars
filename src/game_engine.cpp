#include "game_engine.h"

GameEngine::GameEngine()
{
    createGameWindow();
    gameScenes[currentScene] = std::make_shared<MenuScene>(*this);
}

void GameEngine::startGameLoop()
{
    while (window.isOpen())
    {
        deltaClock.restart();

        update();
        render();
    }
}

void GameEngine::changeScene(Scene::Type sceneType, const std::shared_ptr<Scene>& scene)
{
    currentScene = sceneType;
    gameScenes[currentScene] = scene;
}

void GameEngine::update()
{
    //gameScenes[currentScene]->performAction(); // ??
    gameScenes[currentScene]->update();
}

void GameEngine::render()
{
    gameScenes[currentScene]->render();
}

void GameEngine::createGameWindow()
{
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());

    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;
    window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    if (USE_VERTICAL_SYNC)
    {
        window.setVerticalSyncEnabled(true);
    }
    else
    {
        window.setFramerateLimit(APP_FRAME_RATE);
    }
}
