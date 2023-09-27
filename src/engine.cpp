#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();

    bool isFileLoaded = textureSprite.loadFromFile(BACKGROUND_IMAGE_PATH);
    assert(isFileLoaded);
    backgroundSprite = sf::Sprite(textureSprite);
}

void Engine::startGameLoop()
{
    while (m_window.isOpen())
    {
        deltaClock.restart();

        update();
        render();
    }
}

void Engine::update()
{
    m_entityManager.update();

    m_userInputSystem.execute();
    m_collisionSystem.execute();
    m_entitySpawnerSystem.execute();

    if (guiProperties.hasPaused)
    {
        return;
    }

    m_lifespanSystem.execute();
    m_transformSystem.execute();
}

void Engine::render()
{
    m_window.clear();
    m_window.draw(backgroundSprite);

    if (guiProperties.hasPaused)
    {
        guiProperties.playerRespawnTimeSeconds += deltaClock.getElapsedTime().asSeconds();
        guiProperties.specialAttackCoolDownSeconds += deltaClock.getElapsedTime().asSeconds();
    }

    m_renderSystem.execute();
    m_guiSystem.execute();

    m_window.display();
}

void Engine::createGameWindow()
{
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE.data());

    uint32_t screenWidth = sf::VideoMode::getDesktopMode().width;
    uint32_t screenHeight = sf::VideoMode::getDesktopMode().height;
    m_window.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH)/2, (screenHeight - WINDOW_HEIGHT)/2));

    if (USE_VERTICAL_SYNC)
    {
        m_window.setVerticalSyncEnabled(true);
    }
    else
    {
        m_window.setFramerateLimit(APP_FRAME_RATE);
    }
}