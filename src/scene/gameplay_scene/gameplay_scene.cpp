#include "scene/gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine, GameProperties& gameProperties) : Scene(engine), m_gameProperties(gameProperties)
{
    const std::string backgroundImagePath = m_gameProperties.getSelectedDifficulty() == Difficulty::EASY
            ? LEVEL_ONE_BACKGROUND_IMAGE_PATH
            : m_gameProperties.getSelectedDifficulty() == Difficulty::MEDIUM
                    ? LEVEL_TWO_BACKGROUND_IMAGE_PATH
                    : LEVEL_THREE_BACKGROUND_IMAGE_PATH;

    bool isFileLoaded = textureSprite.loadFromFile(backgroundImagePath);
    assert(isFileLoaded);
    backgroundSprite = sf::Sprite(textureSprite);

    // FIXME hacky
    const Scene::Type sceneType = m_gameProperties.getSelectedDifficulty() == Difficulty::EASY
            ? Scene::Type::LEVEL_ONE_GAMEPLAY_SCENE
            : m_gameProperties.getSelectedDifficulty() == Difficulty::MEDIUM
                    ? Scene::Type::LEVEL_TWO_GAMEPLAY_SCENE
                    : Scene::Type::LEVEL_THREE_GAMEPLAY_SCENE;

    m_audioManager->playMusic(static_cast<uint8_t>(sceneType), 30.0f, false);
    m_gameProperties.resetPropertiesForLevel(levelClock, AudioManager::getInstance()->getCurrentMusicDuration().asSeconds());

    registerActions();
    registerSystems(engine);
}

void GameplayScene::update()
{
    if (levelClock.getElapsedTime().asSeconds() > m_gameProperties.getTimeRemainingBeforeVictory())
    {
        // Return to level screen [DONE]
        // TODO Show snackbar or something that you cleared the level [UNFINISHED]
        // Show tick icon or similar next to level indicating victory [DONE]
        const LevelClearStatus& levelClearStatus = LevelClearStatus(
                m_gameProperties.getSelectedDifficulty() == Difficulty::EASY,
                m_gameProperties.getSelectedDifficulty() == Difficulty::MEDIUM,
                m_gameProperties.getSelectedDifficulty() == Difficulty::HARD);
        changeToLevelSelectScene(levelClearStatus);
        return;
    }
    if (m_gameProperties.getTotalLives() == 0)
    {
        // FIXME Not very clean here, improve
        m_audioManager->stopActiveMusic();
        const std::shared_ptr<GameOverScene>& nextScene = std::make_shared<GameOverScene>(gameEngine);
        gameEngine.changeScene(Scene::Type::GAME_OVER_SCENE, nextScene);
        return;
    }

    m_entityManager.update();
    m_systemManager.update(m_gameProperties);

    if (m_gameProperties.hasPaused())
    {
        m_gameProperties.setPlayerRespawnTimeSeconds(m_gameProperties.getPlayerRespawnTimeSeconds() + DT);
        m_gameProperties.setSpecialAttackCoolDownSeconds(m_gameProperties.getSpecialAttackCoolDownSeconds() + DT);
    }
}

void GameplayScene::render()
{
    gameEngine.window.clear();
    gameEngine.window.draw(backgroundSprite);
    m_systemManager.render(m_gameProperties);

    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    Action::Type actionType = action.getType();
    if (action.getType() == Action::Type::EXIT_SCENE)
    {
        if (action.getMode() == Action::Mode::RELEASE)
        {
            return;
        }

        LevelClearStatus levelClearStatus{};
        changeToLevelSelectScene(levelClearStatus);
    }

    if (actionType == Action::Type::PAUSE && action.getMode() == Action::Mode::PRESS)
    {
        m_gameProperties.setPaused(!m_gameProperties.hasPaused());
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
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }
            m_audioManager->playSound(AudioManager::AudioType::SHOOT, 5.0f);
            actionComponent->projectileDestination = gameEngine.window.mapPixelToCoords(
                    sf::Mouse::getPosition(gameEngine.window));
            actionComponent->isShooting = true;
        }
        if (actionType == Action::Type::SPECIAL_ATTACK)
        {
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }

            if (m_gameProperties.getSpecialAttackCoolDownSeconds() <= m_gameProperties.getLevelClock().getElapsedTime().asSeconds())
            {
                m_audioManager->playSound(AudioManager::AudioType::SPECIAL_ATTACK, 5.0f);
                actionComponent->projectileDestination = gameEngine.window
                        .mapPixelToCoords(sf::Mouse::getPosition(gameEngine.window));
                actionComponent->isPerformingSpecialAttack = true;
            }
        }
    }
}
void GameplayScene::changeToLevelSelectScene(LevelClearStatus levelClearStatus)
{
    m_audioManager->stopActiveMusic();
    const std::shared_ptr<LevelSelectScene>& nextScene = std::make_shared<LevelSelectScene>(gameEngine, levelClearStatus);
    gameEngine.changeScene(Type::LEVEL_SELECT_SCENE, nextScene);
}

void GameplayScene::registerActions()
{
    // mouse
    registerActionType(CURSOR_LEFT, Action::Type::SHOOT);
    registerActionType(CURSOR_RIGHT, Action::Type::SPECIAL_ATTACK);

    // movement
    registerActionType(sf::Keyboard::Left, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Right, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Down, Action::Type::MOVE_DOWN);
    registerActionType(sf::Keyboard::A, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::D, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::W, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::S, Action::Type::MOVE_DOWN);

    // pause
    registerActionType(sf::Keyboard::P, Action::Type::PAUSE);
    registerActionType(sf::Keyboard::Escape, Action::Type::EXIT_SCENE);
}

void GameplayScene::registerSystems(GameEngine& engine)
{
    m_systemManager.registerSystem(std::make_shared<CollisionSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<EntitySpawnSystem>(m_entityManager,  m_gameProperties),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<LifespanSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(std::make_shared<TransformSystem>(m_entityManager),
            SystemManager::SystemType::UPDATE);
    
    m_systemManager.registerSystem(std::make_shared<RenderSystem>(engine.window, m_entityManager),
            SystemManager::SystemType::RENDER);
    m_systemManager.registerSystem(
            std::make_shared<GuiSystem>(engine.window, m_entityManager, m_gameProperties),
            SystemManager::SystemType::RENDER);
}