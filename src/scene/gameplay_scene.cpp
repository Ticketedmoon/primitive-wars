#include "scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
    m_systemManager.registerSystem(
            std::make_shared<UserInputSystem>(engine.window, m_entityManager, engine.worldClock,
                    engine.gameProperties), SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(std::make_shared<CollisionSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(
            std::make_shared<EntitySpawnSystem>(m_entityManager, engine.worldClock, engine.gameProperties),
            SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(std::make_shared<LifespanSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(std::make_shared<TransformSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);

    m_systemManager.registerSystem(std::make_shared<RenderSystem>(engine.window, m_entityManager),
            SystemManager::SystemType::RENDER);

    m_systemManager.registerSystem(
            std::make_shared<GuiSystem>(engine.window, m_entityManager, engine.worldClock, engine.gameProperties),
            SystemManager::SystemType::RENDER);

    bool isFileLoaded = textureSprite.loadFromFile(BACKGROUND_IMAGE_PATH);
    assert(isFileLoaded);
    backgroundSprite = sf::Sprite(textureSprite);
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update(gameEngine.gameProperties);
}

void GameplayScene::render()
{
    gameEngine.window.clear();
    gameEngine.window.draw(backgroundSprite);

    if (gameEngine.gameProperties.hasPaused)
    {
        gameEngine.gameProperties.playerRespawnTimeSeconds += gameEngine.deltaClock.getElapsedTime().asSeconds();
        gameEngine.gameProperties.specialAttackCoolDownSeconds += gameEngine.deltaClock.getElapsedTime().asSeconds();
    }

    m_systemManager.render(gameEngine.gameProperties);

    gameEngine.window.display();
}

void GameplayScene::performAction()
{

}