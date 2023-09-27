#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
}

void Engine::startGameLoop()
{
    while (m_window.isOpen())
    {
        update();
        render();
        frameNo++;
    }
}

void Engine::update()
{
    m_entityManager.update();

    userInputSystem();
    if (hasPaused)
    {
        playerRespawnTimeSeconds = (worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);
        return;
    }

    m_collisionSystem.execute();

    // FIXME Move system(s)?
    bool isPlayerDead = m_entityManager.getEntitiesByType(Entity::Type::PLAYER).empty();
    if (isPlayerDead)
    {
        frameNo = 1;
        if (worldClock.getElapsedTime().asSeconds() > playerRespawnTimeSeconds)
        {
            m_entitySpawnerSystem.spawnPlayer();
        }
    }

    // FIXME Move system(s)?
    if (frameNo % 100 == 0)
    {
        m_entitySpawnerSystem.spawnEnemy();
    }

    // FIXME Move system(s)?
    std::vector<std::shared_ptr<Entity>> destroyedEntities = m_entityManager.getDestroyedEntities();
    for (const auto& e: destroyedEntities)
    {
        if (e->getType() == Entity::Type::PLAYER)
        {
            playerRespawnTimeSeconds = (worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);
            totalDeaths++;
        }

        if (e->getType() == Entity::Type::ENEMY)
        {
            std::shared_ptr<CRender> renderComponent = std::static_pointer_cast<CRender>(
                    e->getComponentByType(Component::Type::RENDER));
            score += (100 * renderComponent->m_shape.getPointCount());
        }
    }

    m_entitySpawnerSystem.execute();
    m_lifespanSystem.execute();
    m_transformSystem.execute();
}

void Engine::render()
{
    m_window.clear();

    m_renderSystem.execute();

    m_window.display();
}

// TODO
//  For laptops or devices with a trackpad or without a mouse, can we add optional ways to shoot?
void Engine::userInputSystem()
{
    sf::Event event{};

    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Key::P)
            {
                hasPaused = !hasPaused;
            }
        }

        if (hasPaused)
        {
            return;
        }

        std::ranges::filter_view view = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
            return e->hasComponent(Component::Type::USER_INPUT);
        });
        std::vector<std::shared_ptr<Entity>> entitiesToUpdate = std::vector(view.begin(), view.end());
        for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
        {
            std::shared_ptr<CUserInput> userInputComponentForEntity = std::static_pointer_cast<CUserInput> (e->getComponentByType(Component::Type::USER_INPUT));

            userInputComponentForEntity->isMovingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            userInputComponentForEntity->isMovingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            userInputComponentForEntity->isMovingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::W);
            userInputComponentForEntity->isMovingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::S);

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
                    sf::Vector2f targetDestinationForBullet = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
                    float h = targetDestinationForBullet.y - transformComponentForEntity->m_position.y;
                    float a = targetDestinationForBullet.x - transformComponentForEntity->m_position.x;
                    double shotAngle = atan2(h, a);
                    m_entitySpawnerSystem.spawnBullet(transformComponentForEntity->m_position, shotAngle);
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (specialAttackCoolDownSeconds > worldClock.getElapsedTime().asSeconds())
                    {
                        return;
                    }

                    m_entitySpawnerSystem.spawnEntityAnimation(e, SpawnProperties(15, Entity::Type::BULLET, true, sf::Vector2f(7.5f, 7.5f)));
                    specialAttackCoolDownSeconds = (worldClock.getElapsedTime().asSeconds() + SPECIAL_ATTACK_COOLDOWN_OFFSET);
                }
            }
        }
    }
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