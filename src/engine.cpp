#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
}

void Engine::startGameLoop()
{
    sf::Clock deltaClock;
    while (m_window.isOpen())
    {
        deltaClock.restart();

        listenForEvents();
        update();
        render();
    }
}

void Engine::listenForEvents()
{
    sf::Event event{};
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

void Engine::update()
{
}

void Engine::render()
{
    m_window.clear();

    // Do something


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