#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();

    bool isFileLoaded = textureSprite.loadFromFile(BACKGROUND_IMAGE_PATH);
    assert(isFileLoaded);
    backgroundSprite = sf::Sprite(textureSprite);

    m_systemManager.registerSystem(std::make_shared<UserInputSystem>(m_window, m_entityManager, worldClock, gameProperties), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<CollisionSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<EntitySpawnSystem>(m_entityManager, worldClock, gameProperties), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<LifespanSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<TransformSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(std::make_shared<RenderSystem>(m_window, m_entityManager), SystemManager::SystemType::RENDER);
    m_systemManager.registerSystem(std::make_shared<GuiSystem>(m_window, m_entityManager, worldClock, gameProperties), SystemManager::SystemType::RENDER);
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
    m_systemManager.update(gameProperties);
}

void Engine::render()
{
    m_window.clear();
    m_window.draw(backgroundSprite);

    if (gameProperties.hasPaused)
    {
        gameProperties.playerRespawnTimeSeconds += deltaClock.getElapsedTime().asSeconds();
        gameProperties.specialAttackCoolDownSeconds += deltaClock.getElapsedTime().asSeconds();
    }

    m_systemManager.render(gameProperties);

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