#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
    configureTextRendering();
    spawnPlayer();
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
    bool isPlayerDead = m_entityManager.getEntitiesByType(Entity::Type::PLAYER).empty();
    if (isPlayerDead && worldClock.getElapsedTime().asSeconds() > playerDeadTimer)
    {
        spawnPlayer();
    }

    userInputSystem();
    if (!hasPaused)
    {
        userInputSystem();
        enemySpawnSystem();
        collisionSystem();
        lifeSpanSystem();
        transformSystem();
    }
}

void Engine::render()
{
    m_window.clear();
    m_window.draw(backgroundSprite);

    renderSystem();

    uint8_t coolDownSeconds = worldClock.getElapsedTime().asSeconds() > specialAttackCoolDown
            ? 0.0f
            : std::ceil(specialAttackCoolDown - worldClock.getElapsedTime().asSeconds());

    const std::string text = "Score: " + std::to_string(score) + "\n"
            + "Deaths: " + std::to_string(totalDeaths) + "\n"
            + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);

    gameOverlayText.setString(text);

    drawText(gameOverlayText, sf::Color::Yellow, 24, sf::Vector2f(24, 12));

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        uint8_t respawnTime = (playerDeadTimer - worldClock.getElapsedTime().asSeconds()) + 1;
        respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
        drawText(respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 324, WINDOW_HEIGHT / 2 - 64));
    }

    if (hasPaused)
    {
        drawText(pauseText, sf::Color::Green, 128, CENTRE_SCREEN_POSITION);
    }

    m_window.display();
}

void Engine::transformSystem()
{
    std::vector<std::shared_ptr<Entity>> playersToUpdate = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    for (const std::shared_ptr<Entity>& e: playersToUpdate)
    {
        std::shared_ptr<CTransform> transformComponent = std::static_pointer_cast<CTransform>(e->getComponentByType(
                Component::Type::TRANSFORM));
        std::shared_ptr<CCollision> collisionComponent = std::static_pointer_cast<CCollision>(e->getComponentByType(
                Component::Type::COLLISION));
        std::shared_ptr<CUserInput> userInputComponent = std::static_pointer_cast<CUserInput>(e->getComponentByType(
                Component::Type::USER_INPUT));

        if (!collisionComponent->isCollidingLeft)
        {
            transformComponent->m_position.x -= userInputComponent->isMovingLeft
                    ? transformComponent->m_speed.x
                    : 0;
        }
        if (!collisionComponent->isCollidingRight)
        {
            transformComponent->m_position.x += userInputComponent->isMovingRight
                    ? transformComponent->m_speed.x
                    : 0;
        }
        if (!collisionComponent->isCollidingUp)
        {
            transformComponent->m_position.y -= userInputComponent->isMovingUp
                    ? transformComponent->m_speed.y
                    : 0;
        }
        if (!collisionComponent->isCollidingDown)
        {
            transformComponent->m_position.y += userInputComponent->isMovingDown
                    ? transformComponent->m_speed.y
                    : 0;
        }
    }

    std::vector<std::shared_ptr<Entity>> bulletsToUpdate = m_entityManager.getEntitiesByType(Entity::Type::BULLET);
    for (const std::shared_ptr<Entity>& bullet: bulletsToUpdate)
    {
        std::shared_ptr <CTransform> transformComponent = std::static_pointer_cast<CTransform>(bullet->getComponentByType(
                Component::Type::TRANSFORM));
        std::shared_ptr <CCollision> collisionComponent = std::static_pointer_cast<CCollision>(bullet->getComponentByType(
                Component::Type::COLLISION));

        transformComponent->m_position.x += transformComponent->m_speed.x * transformComponent->m_speedDelta.x;
        transformComponent->m_position.y += transformComponent->m_speed.y * transformComponent->m_speedDelta.y;
    }

    std::vector <std::shared_ptr<Entity>> enemiesToUpdate = m_entityManager.getEntitiesByType(Entity::Type::ENEMY);
    for (const std::shared_ptr <Entity>& e: enemiesToUpdate)
    {
        std::shared_ptr <CTransform> transformComponent = std::static_pointer_cast<CTransform>(e->getComponentByType(
                Component::Type::TRANSFORM));

        if (e->hasComponent(Component::Type::COLLISION))
        {
            std::shared_ptr <CCollision> collisionComponent = std::static_pointer_cast<CCollision>(e->getComponentByType(
                    Component::Type::COLLISION));
            if (collisionComponent->isCollidingLeft || collisionComponent->isCollidingRight)
            {
                transformComponent->m_speed.x = -transformComponent->m_speed.x;
            }
            if (collisionComponent->isCollidingUp || collisionComponent->isCollidingDown)
            {
                transformComponent->m_speed.y = -transformComponent->m_speed.y;
            }
        }

        transformComponent->m_position.x += (transformComponent->m_speed.x * transformComponent->m_speedDelta.x);
        transformComponent->m_position.y += (transformComponent->m_speed.y * transformComponent->m_speedDelta.y);
    }
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
                    spawnBullet(transformComponentForEntity->m_position, shotAngle);
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (specialAttackCoolDown > worldClock.getElapsedTime().asSeconds())
                    {
                        return;
                    }

                    float PI = std::numbers::pi_v<float> * 2;
                    size_t totalVertices = 15;

                    for (size_t i = 0; i < totalVertices; i++)
                    {
                        double shotAngle = (PI / totalVertices) * i;
                        const SpawnProperties& properties = SpawnProperties(Entity::Type::BULLET, shotAngle, true, sf::Vector2f(7.5f, 7.5f));
                        spawnEntityClone(e, properties);
                    }

                    specialAttackCoolDown = (worldClock.getElapsedTime().asSeconds() + SPECIAL_ATTACK_COOLDOWN_OFFSET);
                }
            }
        }
    }
}

void Engine::enemySpawnSystem()
{
    if (frameNo % 100 == 0)
    {
        // spawn enemy
        spawnEnemy();
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
                // kill enemy
                enemy->isAlive = false;

                // destroy bullet
                bullet->isAlive = false;

                // update score
                size_t totalVertices = enemyRenderComponent->m_shape.getPointCount();
                score += (100 * totalVertices);

                // animation
                float PI = std::numbers::pi_v<float> * 2;
                for (size_t i = 0; i < totalVertices; i++)
                {
                    double shotAngle = (PI / totalVertices) * i;
                    const SpawnProperties& properties = SpawnProperties(Entity::Type::ENEMY, shotAngle, false, sf::Vector2f(2.0f, 2.0f));
                    spawnEntityClone(enemy, properties);
                }
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
                // kill player
                player->isAlive = false;

                playerDeadTimer = (worldClock.getElapsedTime().asSeconds() + PLAYER_DEAD_TIME_OFFSET);

                // kill enemy
                enemy->isAlive = false;

                // animation
                size_t totalVertices = enemyRenderComponent->m_shape.getPointCount();
                float PI = std::numbers::pi_v<float> * 2;
                for (size_t i = 0; i < totalVertices; i++)
                {
                    double shotAngle = (PI / totalVertices) * i;
                    const SpawnProperties& properties = SpawnProperties(Entity::Type::ENEMY, shotAngle, false, sf::Vector2f(2.0f, 2.0f));
                    spawnEntityClone(enemy, properties);
                }

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
            e->isAlive = false;
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

void Engine::spawnPlayer()
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    const sf::Vector2f& position = sf::Vector2f(10.0f, 10.0f);
    sf::CircleShape shape = createShape({20.0f, 3, position, sf::Color::Black, sf::Color::Red, 5.0f});

    player->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), position);
    player->m_components[Component::Type::COLLISION] = std::make_shared<CCollision>();
    player->m_components[Component::Type::USER_INPUT] = std::make_shared<CUserInput>();
    player->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void Engine::spawnEnemy()
{
    std::shared_ptr<Entity>& enemy = m_entityManager.addEntity(Entity::Type::ENEMY);

    const int radius = std::experimental::randint(10, 50);
    const int x = std::experimental::randint(radius, static_cast<int>(WINDOW_WIDTH - radius));
    const int y = std::experimental::randint(radius, static_cast<int>(WINDOW_HEIGHT - radius));

    const sf::Vector2f position = isNearPlayer(sf::FloatRect(x, y, radius, radius))
            ? sf::Vector2f(radius * 2, radius * 2)
            : sf::Vector2f(x, y);
    const uint8_t totalVertices = std::experimental::randint(3, 10);
    const sf::Color fillColor = sf::Color(std::experimental::randint(50, 255), std::experimental::randint(50, 255),
            std::experimental::randint(50, 255), 255);

    sf::CircleShape shape = createShape({static_cast<float>(radius), totalVertices, position, fillColor, sf::Color::White, 3.0f});

    enemy->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(shape.getPosition(),
            sf::Vector2f(std::experimental::randint(1, 3), std::experimental::randint(1, 3)), sf::Vector2f(1, 1));
    enemy->m_components[Component::Type::COLLISION] = std::make_shared<CCollision>();
    enemy->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void Engine::spawnEntityClone(const std::shared_ptr<Entity>& existingEnemy, SpawnProperties spawnProperties)
{
    std::shared_ptr<Entity>& enemyDeathAnimationEntity = m_entityManager.addEntity(spawnProperties.entityType);

    sf::CircleShape shapeForExistingEnemy =
            std::static_pointer_cast<CRender>(existingEnemy->getComponentByType(Component::Type::RENDER))->m_shape;

    float radius = shapeForExistingEnemy.getRadius() / 3;
    sf::CircleShape shape = createShape({radius, shapeForExistingEnemy.getPointCount(), shapeForExistingEnemy.getPosition(),
                                         shapeForExistingEnemy.getFillColor(), shapeForExistingEnemy.getOutlineColor(),
                                         shapeForExistingEnemy.getOutlineThickness()});

    sf::Vector2f enemyAnimationPosition =
            std::static_pointer_cast<CTransform>(existingEnemy->getComponentByType(Component::Type::TRANSFORM))->m_position;

    enemyDeathAnimationEntity->m_components[Component::Type::COLLISION] = spawnProperties.isCollidable ? std::make_shared<CCollision>() : nullptr;
    enemyDeathAnimationEntity->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(enemyAnimationPosition,
            spawnProperties.speed, sf::Vector2f(cos(spawnProperties.shotAngle) * 1.0f, sin(spawnProperties.shotAngle) * 1.0f));
    enemyDeathAnimationEntity->m_components[Component::Type::LIFESPAN] = std::make_shared<CLifespan>(100);
    enemyDeathAnimationEntity->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void Engine::spawnBullet(sf::Vector2f position, double shotAngle)
{
    std::shared_ptr<Entity>& e = m_entityManager.addEntity(Entity::Type::BULLET);

    sf::CircleShape shape = createShape({10, 32, position, sf::Color::White, sf::Color::Black, 1.0f});

    e->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(position, sf::Vector2f(10.0f, 10.0f),
            sf::Vector2f(cos(shotAngle) * 1.0f, sin(shotAngle) * 1.0f));
    e->m_components[Component::Type::COLLISION] = std::make_shared<CCollision>();
    e->m_components[Component::Type::LIFESPAN] = std::make_shared<CLifespan>(100);
    e->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

sf::CircleShape Engine::createShape(ShapeProperties properties)
{
    float radius = properties.radius;
    sf::CircleShape shape(radius, properties.totalVertices);
    shape.setOrigin(radius, radius);
    shape.setPosition(properties.position);
    shape.setFillColor(properties.fillColor);
    shape.setOutlineColor(properties.outlineColor);
    shape.setOutlineThickness(properties.outlineThickness);
    return shape;
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

    const std::string texturePath = "resources/assets/board.png";
    bool isFileLoaded = textureSprite.loadFromFile(texturePath);
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

/*
 * This is about nearness and not collision.
 * We do not want an enemy to spawn on-top or even close-by to the player as this is either impossible or very difficult
 * to react to, and does not provide a good user experience.
 */
bool Engine::isNearPlayer(sf::FloatRect enemyBoundingBox)
{
    const std::shared_ptr<CRender>& renderComponentForPlayer = std::static_pointer_cast<CRender>(
            m_entityManager.getEntityByType(Entity::Type::PLAYER)->getComponentByType(Component::Type::RENDER));
    bool isPlayerDead = m_entityManager.getEntitiesByType(Entity::Type::PLAYER).empty();
    if (isPlayerDead)
    {
        return false;
    }

    // Make player rect larger for this calculation so enemies are not 'near'
    int offsetPx = 256;
    sf::FloatRect playerBoundingBox = renderComponentForPlayer->m_shape.getGlobalBounds();
    playerBoundingBox.left = (playerBoundingBox.left - offsetPx) < 0 ? 0 : (playerBoundingBox.left - offsetPx);
    playerBoundingBox.top = (playerBoundingBox.top - offsetPx) < 0 ? 0 : (playerBoundingBox.top - offsetPx);
    playerBoundingBox.width = playerBoundingBox.width + offsetPx;
    playerBoundingBox.height = playerBoundingBox.height + offsetPx;
    return playerBoundingBox.contains(sf::Vector2f(enemyBoundingBox.left, enemyBoundingBox.top));
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