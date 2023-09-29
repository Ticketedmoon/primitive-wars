#include "scene/gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine)
{
    // mouse
    registerActionType(CursorButton::CURSOR_LEFT, Action::Type::SHOOT);
    registerActionType(CursorButton::CURSOR_RIGHT, Action::Type::SPECIAL_ATTACK);

    // movement
    registerActionType(sf::Keyboard::Key::Left, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Key::Right, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Key::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::Down, Action::Type::MOVE_DOWN);
    registerActionType(sf::Keyboard::Key::A, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Key::D, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Key::W, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::S, Action::Type::MOVE_DOWN);

    // pause
    registerActionType(sf::Keyboard::Key::P, Action::Type::PAUSE);

    registerSystems(engine);

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

void GameplayScene::performAction(Action& action)
{
    if (action.getType() == Action::Type::PAUSE && action.getMode() == Action::Mode::PRESS)
    {
        gameEngine.gameProperties.hasPaused = !gameEngine.gameProperties.hasPaused;
    }

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CAction> userActionComponent = std::static_pointer_cast<CAction>(
                player->getComponentByType(Component::Type::USER_INPUT));
        if (action.getType() == Action::Type::MOVE_LEFT)
        {
            userActionComponent->isMovingLeft = action.getMode() == Action::Mode::PRESS;
        }
        if (action.getType() == Action::Type::MOVE_RIGHT)
        {
            userActionComponent->isMovingRight = action.getMode() == Action::Mode::PRESS;
        }
        if (action.getType() == Action::Type::MOVE_UP)
        {
            userActionComponent->isMovingUp = action.getMode() == Action::Mode::PRESS;
        }
        if (action.getType() == Action::Type::MOVE_DOWN)
        {
            userActionComponent->isMovingDown = action.getMode() == Action::Mode::PRESS;
        }
        if (action.getType() == Action::Type::SHOOT)
        {
            userActionComponent->projectileDestination = gameEngine.window.mapPixelToCoords(
                    sf::Mouse::getPosition(gameEngine.window));
            userActionComponent->isShooting = action.getMode() == Action::Mode::PRESS;
        }
        if (action.getType() == Action::Type::SPECIAL_ATTACK)
        {
            if (gameEngine.gameProperties.specialAttackCoolDownSeconds <= gameEngine.gameProperties.worldClock.getElapsedTime().asSeconds())
            {
                userActionComponent->projectileDestination = gameEngine.window
                        .mapPixelToCoords(sf::Mouse::getPosition(gameEngine.window));
                userActionComponent->isPerformingSpecialAttack = action.getMode() == Action::Mode::PRESS;
            }
        }
    }
}

void GameplayScene::registerSystems(GameEngine& engine)
{
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
}