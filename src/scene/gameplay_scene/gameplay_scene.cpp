#include "scene/gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine, GameProperties& gameProperties) : Scene(engine), m_gameProperties(gameProperties)
{
    const std::string backgroundImagePath = m_gameProperties.difficulty == Difficulty::EASY
            ? LEVEL_ONE_BACKGROUND_IMAGE_PATH
            : m_gameProperties.difficulty == Difficulty::MEDIUM ? LEVEL_TWO_BACKGROUND_IMAGE_PATH : LEVEL_THREE_BACKGROUND_IMAGE_PATH;

    bool isFileLoaded = textureSprite.loadFromFile(backgroundImagePath);
    assert(isFileLoaded);
    backgroundSprite = sf::Sprite(textureSprite);

    // FIXME hacky
    const Scene::Type sceneType = m_gameProperties.difficulty == Difficulty::EASY
            ? Scene::Type::LEVEL_ONE_GAMEPLAY_SCENE
            : m_gameProperties.difficulty == Difficulty::MEDIUM
                    ? Scene::Type::LEVEL_TWO_GAMEPLAY_SCENE
                    : Scene::Type::LEVEL_THREE_GAMEPLAY_SCENE;

    m_audioManager->playMusic(static_cast<uint8_t>(sceneType), 30.0f, false);

    float timeRemainingBeforeLevelComplete = levelClock.getElapsedTime().asSeconds() +
            AudioManager::getInstance()->getCurrentMusicDuration().asSeconds();

    gameProperties.worldClock = levelClock;
    gameProperties.hasPaused = false;
    gameProperties.totalLives = 3;
    gameProperties.playerRespawnTimeSeconds = 0;
    gameProperties.specialAttackCoolDownSeconds = 0;
    gameProperties.timeRemainingBeforeVictory = timeRemainingBeforeLevelComplete;
    m_gameProperties = gameProperties;

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
}

void GameplayScene::update()
{
    if (levelClock.getElapsedTime().asSeconds() > m_gameProperties.timeRemainingBeforeVictory)
    {
        // Return to level screen [DONE]
        // TODO Show snackbar or something that you cleared the level [UNFINISHED]
        // Show tick icon or similar next to level indicating victory [DONE]
        m_audioManager->stopActiveMusic();
        const std::shared_ptr<LevelSelectScene>& nextScene = std::make_shared<LevelSelectScene>(gameEngine,
                LevelSelectScene::LevelClearStatus(m_gameProperties.difficulty == Difficulty::EASY,
                        m_gameProperties.difficulty == Difficulty::MEDIUM,
                        m_gameProperties.difficulty == Difficulty::HARD));
        gameEngine.changeScene(Scene::Type::LEVEL_SELECT_SCENE, nextScene);
        return;
    }
    if (m_gameProperties.totalLives == 0)
    {
        // FIXME Not very clean here, improve
        m_audioManager->stopActiveMusic();
        const std::shared_ptr<GameOverScene>& nextScene = std::make_shared<GameOverScene>(gameEngine);
        gameEngine.changeScene(Scene::Type::GAME_OVER_SCENE, nextScene);
        return;
    }

    m_entityManager.update();
    m_systemManager.update(m_gameProperties);
}

void GameplayScene::render()
{
    gameEngine.window.clear();
    gameEngine.window.draw(backgroundSprite);

    if (m_gameProperties.hasPaused)
    {
        m_gameProperties.playerRespawnTimeSeconds += gameEngine.deltaClock.getElapsedTime().asSeconds();
        m_gameProperties.specialAttackCoolDownSeconds += gameEngine.deltaClock.getElapsedTime().asSeconds();
    }

    m_systemManager.render(m_gameProperties);

    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    Action::Type actionType = action.getType();
    if (actionType == Action::Type::PAUSE && action.getMode() == Action::Mode::PRESS)
    {
        m_gameProperties.hasPaused = !m_gameProperties.hasPaused;
    }

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    for (std::shared_ptr<Entity>& player : players)
    {
        std::shared_ptr<CAction> actionComponent = std::static_pointer_cast<CAction>(
                player->getComponentByType(Component::Type::USER_INPUT));
        if (actionType == Action::Type::MOVE_LEFT)
        {
            actionComponent->isMovingLeft = action.getMode() == Action::Mode::PRESS;
        }
        if (actionType == Action::Type::MOVE_RIGHT)
        {
            actionComponent->isMovingRight = action.getMode() == Action::Mode::PRESS;
        }
        if (actionType == Action::Type::MOVE_UP)
        {
            actionComponent->isMovingUp = action.getMode() == Action::Mode::PRESS;
        }
        if (actionType == Action::Type::MOVE_DOWN)
        {
            actionComponent->isMovingDown = action.getMode() == Action::Mode::PRESS;
        }
        if (actionType == Action::Type::SHOOT)
        {
            if (action.getMode() == Action::Mode::PRESS)
            {
                m_audioManager->playSound(AudioManager::AudioType::SHOOT, 5.0f);
            }
            actionComponent->projectileDestination = gameEngine.window.mapPixelToCoords(
                    sf::Mouse::getPosition(gameEngine.window));
            actionComponent->isShooting = action.getMode() == Action::Mode::PRESS;
        }
        if (actionType == Action::Type::SPECIAL_ATTACK)
        {
            if (m_gameProperties.specialAttackCoolDownSeconds <= m_gameProperties.worldClock.getElapsedTime().asSeconds())
            {
                m_audioManager->playSound(AudioManager::AudioType::SPECIAL_ATTACK, 5.0f);
                actionComponent->projectileDestination = gameEngine.window
                        .mapPixelToCoords(sf::Mouse::getPosition(gameEngine.window));
                actionComponent->isPerformingSpecialAttack = action.getMode() == Action::Mode::PRESS;
            }
        }
    }
}

void GameplayScene::registerSystems(GameEngine& engine)
{
    m_systemManager.registerSystem(std::make_shared<CollisionSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<EntitySpawnSystem>(m_entityManager, levelClock, m_gameProperties),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<LifespanSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<TransformSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);
    
    m_systemManager.registerSystem(std::make_shared<RenderSystem>(engine.window, m_entityManager),
            SystemManager::SystemType::RENDER);
    m_systemManager.registerSystem(
            std::make_shared<GuiSystem>(engine.window, m_entityManager, levelClock, m_gameProperties),
            SystemManager::SystemType::RENDER);
}