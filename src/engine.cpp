#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
    configureTextRendering();
    m_spawnerSystem.spawnPlayer();
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
    if (!hasPaused)
    {
        playerRespawnSystem();
        enemySpawnSystem();
        collisionSystem();
        lifeSpanSystem();
        m_transformSystem.execute();
    }
}

void Engine::render()
{
    m_window.clear();
    m_window.draw(backgroundSprite);

    renderSystem();

    uint8_t coolDownSeconds = worldClock.getElapsedTime().asSeconds() > specialAttackCoolDownSeconds
            ? 0.0f
            : std::ceil(specialAttackCoolDownSeconds - worldClock.getElapsedTime().asSeconds());

    const std::string text = "Score: " + std::to_string(score) + "\n"
            + "Deaths: " + std::to_string(totalDeaths) + "\n"
            + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);

    gameOverlayText.setString(text);

    drawText(gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);

    if (hasPaused)
    {
        drawText(pauseText, sf::Color::Green, 128, CENTRE_SCREEN_POSITION);
        playerRespawnTimeSeconds = (worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);
    }
    else if (players.empty())
    {
        uint8_t respawnTime = (playerRespawnTimeSeconds - worldClock.getElapsedTime().asSeconds()) + 1;
        respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
        drawText(respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 324, WINDOW_HEIGHT / 2 - 64));
    }
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
                    m_spawnerSystem.spawnBullet(transformComponentForEntity->m_position, shotAngle);
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (specialAttackCoolDownSeconds > worldClock.getElapsedTime().asSeconds())
                    {
                        return;
                    }

                    m_spawnerSystem.spawnEntityAnimation(e, SpawnProperties(15, Entity::Type::BULLET, true, sf::Vector2f(7.5f, 7.5f)));
                    specialAttackCoolDownSeconds = (worldClock.getElapsedTime().asSeconds() + SPECIAL_ATTACK_COOLDOWN_OFFSET);
                }
            }
        }
    }
}

void Engine::playerRespawnSystem()
{
    bool isPlayerDead = m_entityManager.getEntitiesByType(Entity::Type::PLAYER).empty();
    if (isPlayerDead)
    {
        frameNo = 1;
        if (worldClock.getElapsedTime().asSeconds() >= playerRespawnTimeSeconds)
        {
            m_spawnerSystem.spawnPlayer();
        }
    }
}

void Engine::enemySpawnSystem()
{
    if (frameNo % 100 == 0)
    {
        // spawn enemy
        m_spawnerSystem.spawnEnemy();
    }
}

void Engine::collisionSystem()
{
    std::vector<std::shared_ptr<Entity>> bulletsToUpdate = m_entityManager.getEntitiesByType(Entity::Type::BULLET);
    for (const std::shared_ptr<Entity>& bullet: bulletsToUpdate)
    {
        std::shared_ptr<CRender> bulletRenderComponent = std::static_pointer_cast<CRender>(bullet->getComponentByType(Component::Type::RENDER));
        std::ranges::filter_view enemiesFiltered = m_entityManager.getEntitiesByType(Entity::Type::ENEMY) |
                std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
                    return e->hasComponent(Component::Type::TRANSFORM) &&
                    e->hasComponent(Component::Type::COLLISION) &&
                    e->hasComponent(Component::Type::RENDER);
                });
        std::vector<std::shared_ptr<Entity>> enemiesToUpdate = std::vector(enemiesFiltered.begin(), enemiesFiltered.end());
        for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
        {
            std::shared_ptr<CTransform> transformComponentForEnemy = std::static_pointer_cast<CTransform>(enemy->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CRender> enemyRenderComponent = std::static_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            if (isCollidingAABB(bulletRenderComponent, enemyRenderComponent))
            {
                // destroy enemy
                enemy->destroy();

                // destroy bullet
                bullet->destroy();

                // update score
                size_t totalVertices = enemyRenderComponent->m_shape.getPointCount();
                score += (100 * totalVertices);

                m_spawnerSystem.spawnEntityAnimation(enemy, SpawnProperties(totalVertices, Entity::Type::ENEMY, false, sf::Vector2f(2.0f, 2.0f)));
            }
        }
    }

    std::ranges::filter_view playersFiltered = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::RENDER);
    });
    std::ranges::filter_view enemiesFiltered = m_entityManager.getEntitiesByType(Entity::Type::ENEMY) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::RENDER);
    });

    std::vector<std::shared_ptr<Entity>> enemiesToUpdate = std::vector(enemiesFiltered.begin(), enemiesFiltered.end());
    std::vector<std::shared_ptr<Entity>> playersToUpdate = std::vector(playersFiltered.begin(), playersFiltered.end());

    for (const std::shared_ptr<Entity>& player: playersToUpdate)
    {
        checkForWindowCollision(player);
        for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
        {
            std::shared_ptr<CRender> playerRenderComponent = std::static_pointer_cast<CRender>(player->getComponentByType(Component::Type::RENDER));
            std::shared_ptr<CRender> enemyRenderComponent = std::static_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            if (isCollidingAABB(playerRenderComponent, enemyRenderComponent))
            {
                // destroy player
                player->destroy();

                playerRespawnTimeSeconds = (worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);

                // destroy enemy
                enemy->destroy();

                m_spawnerSystem.spawnEntityAnimation(enemy, SpawnProperties(enemyRenderComponent->m_shape.getPointCount(), Entity::Type::ENEMY, false, sf::Vector2f(2.0f, 2.0f)));

                totalDeaths++;
            }
        }
    }

    for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
    {
        checkForWindowCollision(enemy);
    }
}

void Engine::checkForWindowCollision(const std::shared_ptr<Entity>& e)
{
    std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> collisionComponentForEntity = std::static_pointer_cast<CCollision>(e->getComponentByType(Component::Type::COLLISION));
    if (collisionComponentForEntity != nullptr)
    {
        std::shared_ptr<CRender> renderComponentForEntity = std::static_pointer_cast<CRender>(e->getComponentByType(Component::Type::RENDER));

        collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->m_position.x <=
                renderComponentForEntity->m_shape.getRadius();

        collisionComponentForEntity->isCollidingRight = transformComponentForEntity->m_position.x >=
                WINDOW_WIDTH - renderComponentForEntity->m_shape.getRadius();

        collisionComponentForEntity->isCollidingUp = transformComponentForEntity->m_position.y <=
                renderComponentForEntity->m_shape.getRadius();

        collisionComponentForEntity->isCollidingDown = transformComponentForEntity->m_position.y >=
                WINDOW_HEIGHT - renderComponentForEntity->m_shape.getRadius();
    }
}

void Engine::lifeSpanSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::LIFESPAN) && e->hasComponent(Component::Type::RENDER);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CLifespan> lifeSpanComponent = std::static_pointer_cast<CLifespan> (e->getComponentByType(Component::Type::LIFESPAN));

        if (!lifeSpanComponent->isAlive())
        {
            e->destroy();
        }
        else
        {
            std::shared_ptr<CRender> renderComponent = std::static_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));
            sf::Color fillColor = renderComponent->m_shape.getFillColor();
            sf::Color outlineColor = renderComponent->m_shape.getOutlineColor();
            fillColor.a -= fillColor.a >= 2.55 ? 2.55 : 0;
            outlineColor.a -= outlineColor.a >= 2.55 ? 2.55 : 0;

            renderComponent->m_shape.setFillColor(fillColor);
            renderComponent->m_shape.setOutlineColor(outlineColor);

            lifeSpanComponent->decreaseTimeToLive();
        }
    }
}

void Engine::renderSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::RENDER) && e->hasComponent(Component::Type::TRANSFORM);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CRender> renderComponentForEntity = std::static_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));

        sf::CircleShape& drawable = renderComponentForEntity->m_shape;
        drawable.setPosition(transformComponentForEntity->m_position);
        drawable.rotate(2);

        m_window.draw(drawable);
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

    bool isFileLoaded = textureSprite.loadFromFile(BACKGROUND_IMAGE_PATH);
    assert(isFileLoaded);

    backgroundSprite = sf::Sprite(textureSprite);
}

bool Engine::isCollidingAABB(
        const std::shared_ptr<CRender>& renderComponentForEntity,
        const std::shared_ptr<CRender>& renderComponentForEnemy)
{
    return renderComponentForEntity->m_shape.getGlobalBounds()
        .intersects(renderComponentForEnemy->m_shape.getGlobalBounds());
}

void Engine::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    // TODO parameterise us
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3.0f);
    text.setLetterSpacing(3.0f);

    m_window.draw(text);
}

void Engine::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    gameOverlayText = sf::Text("", m_font);
    respawnText = sf::Text("", m_font);
    pauseText = sf::Text(PAUSED_TEXT, m_font);
}