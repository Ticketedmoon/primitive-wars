#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();

    configureTextRendering();
}

void Engine::startGameLoop()
{
    spawnPlayer();

    while (m_window.isOpen())
    {
        listenForEvents();
        update();
        render();

        frameNo++;
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
    m_entityManager.update();

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        spawnPlayer();
    }

    userInputSystem();
    enemySpawnSystem();
    collisionSystem();
    lifeSpanSystem();
    transformSystem();
}

void Engine::render()
{
    m_window.clear();
    m_window.draw(backgroundSprite);
    const std::string& text = "Score: " + std::to_string(score) + "\nDeaths: " + std::to_string(totalDeaths);
    drawText(text, sf::Color::White, 24, sf::Vector2f(20, 20));

    renderSystem();

    m_window.display();
}

void Engine::transformSystem()
{
    std::ranges::filter_view playersFiltered = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) |
            std::ranges::views::filter([](std::shared_ptr <Entity>& e) {
                return e->hasComponent(Component::Type::TRANSFORM) &&
                e->hasComponent(Component::Type::COLLISION);
            });
    std::vector <std::shared_ptr<Entity>> playersToUpdate = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    for (const std::shared_ptr <Entity>& e: playersToUpdate)
    {
        std::shared_ptr <CTransform> transformComponent = std::dynamic_pointer_cast<CTransform>(e->getComponentByType(
                Component::Type::TRANSFORM));
        std::shared_ptr <CCollision> collisionComponent = std::dynamic_pointer_cast<CCollision>(e->getComponentByType(
                Component::Type::COLLISION));
        std::shared_ptr <CUserInput> userInputComponent = std::dynamic_pointer_cast<CUserInput>(e->getComponentByType(
                Component::Type::USER_INPUT));

        if (!collisionComponent->isCollidingLeft)
        {
            transformComponent->position.x -= userInputComponent->isMovingLeft
                    ? transformComponent->speed.x
                    : 0;
        }
        if (!collisionComponent->isCollidingRight)
        {
            transformComponent->position.x += userInputComponent->isMovingRight
                    ? transformComponent->speed.x
                    : 0;
        }
        if (!collisionComponent->isCollidingUp)
        {
            transformComponent->position.y -= userInputComponent->isMovingUp
                    ? transformComponent->speed.y
                    : 0;
        }
        if (!collisionComponent->isCollidingDown)
        {
            transformComponent->position.y += userInputComponent->isMovingDown
                    ? transformComponent->speed.y
                    : 0;
        }
    }

    std::vector<std::shared_ptr<Entity>> bulletsToUpdate = m_entityManager.getEntitiesByType(Entity::Type::BULLET);
    for (const std::shared_ptr<Entity>& bullet: bulletsToUpdate)
    {
        std::shared_ptr <CTransform> transformComponent = std::dynamic_pointer_cast<CTransform>(bullet->getComponentByType(
                Component::Type::TRANSFORM));
        std::shared_ptr <CCollision> collisionComponent = std::dynamic_pointer_cast<CCollision>(bullet->getComponentByType(
                Component::Type::COLLISION));

        transformComponent->position.x += transformComponent->speed.x * transformComponent->speedDelta.x;
        transformComponent->position.y += transformComponent->speed.y * transformComponent->speedDelta.y;
    }

    std::vector <std::shared_ptr<Entity>> enemiesToUpdate = m_entityManager.getEntitiesByType(Entity::Type::ENEMY);
    for (const std::shared_ptr <Entity>& e: enemiesToUpdate)
    {
        std::shared_ptr <CTransform> transformComponent = std::dynamic_pointer_cast<CTransform>(e->getComponentByType(
                Component::Type::TRANSFORM));
        std::shared_ptr <CCollision> collisionComponent = std::dynamic_pointer_cast<CCollision>(e->getComponentByType(
                Component::Type::COLLISION));

        if (collisionComponent->isCollidingLeft || collisionComponent->isCollidingRight)
        {
            transformComponent->speed.x = -transformComponent->speed.x;
        }
        if (collisionComponent->isCollidingUp || collisionComponent->isCollidingDown)
        {
            transformComponent->speed.y = -transformComponent->speed.y;
        }

        transformComponent->position.x += transformComponent->speed.x;
        transformComponent->position.y -= transformComponent->speed.y;
    }
}

void Engine::userInputSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::USER_INPUT);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CUserInput> userInputComponentForEntity = std::dynamic_pointer_cast<CUserInput> (e->getComponentByType(Component::Type::USER_INPUT));
        std::shared_ptr<CTransform> transformComponentForEntity = std::dynamic_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));

        userInputComponentForEntity->isMovingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        userInputComponentForEntity->isMovingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        userInputComponentForEntity->isMovingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        userInputComponentForEntity->isMovingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // TODO New component?
            if (!userInputComponentForEntity->isMousePressed)
            {
                spawnBullet(transformComponentForEntity->position);
            }
            userInputComponentForEntity->isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        }
        else
        {
            userInputComponentForEntity->isMousePressed = false;
        }



        // TODO for laptops or devices with a trackpad or without a mouse, can we add optional ways to shoot?
        //      - perhaps with the arrow keys
    }
}

void Engine::enemySpawnSystem()
{
    if (frameNo % 250 == 0)
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
        std::shared_ptr<CRender> renderComponentForEntity = std::dynamic_pointer_cast<CRender>(bullet->getComponentByType(Component::Type::RENDER));
        std::ranges::filter_view enemiesFiltered = m_entityManager.getEntitiesByType(Entity::Type::ENEMY) |
                std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
                    return e->hasComponent(Component::Type::TRANSFORM) &&
                    e->hasComponent(Component::Type::COLLISION) &&
                    e->hasComponent(Component::Type::RENDER);
                });
        std::vector<std::shared_ptr<Entity>> enemiesToUpdate = std::vector(enemiesFiltered.begin(), enemiesFiltered.end());
        for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
        {
            std::shared_ptr<CTransform> transformComponentForEnemy = std::dynamic_pointer_cast<CTransform>(enemy->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CRender> renderComponentForEnemy = std::dynamic_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            if (isCollidingAABB(renderComponentForEntity, renderComponentForEnemy))
            {
                // kill enemy
                enemy->isAlive = false;

                // destroy bullet
                bullet->isAlive = false;

                // update score
                size_t totalVertices = renderComponentForEnemy->renderBody.getPointCount();
                score += (100 * totalVertices);

                // animation
                for (int i = 1; i <= totalVertices; i++)
                {
                    std::shared_ptr<Entity>& entity = m_entityManager.addEntity(Entity::Type::ENEMY);

                    std::shared_ptr<CTransform> cTransform = std::make_shared<CTransform>();
                    cTransform->position = sf::Vector2f(transformComponentForEnemy->position.x + (i * 10),
                            transformComponentForEnemy->position.y + (i * -10));
                    //cTransform->speed = sf::Vector2f(5, 5);
                    std::shared_ptr<CCollision> cCollision = std::make_shared<CCollision>();
                    std::shared_ptr<CLifespan> cLifespan = std::make_shared<CLifespan>(50);

                    float radius = 30.0f;
                    sf::CircleShape shape(radius, 8);
                    shape.setOrigin(radius, radius);
                    shape.setPosition(cTransform->position);
                    shape.setFillColor(sf::Color::Transparent);
                    shape.setOutlineColor(sf::Color::Red);
                    shape.setOutlineThickness(3.0f);

                    std::shared_ptr<CRender> cRender = std::make_shared<CRender>();
                    cRender->renderBody = shape;

                    std::pair transformPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::TRANSFORM, cTransform);
                    std::pair collisionPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::COLLISION, cCollision);
                    std::pair userInputPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::LIFESPAN, cLifespan);
                    std::pair renderPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::RENDER, cRender);

                    entity->m_componentsByType.insert(transformPair);
                    entity->m_componentsByType.insert(collisionPair);
                    entity->m_componentsByType.insert(userInputPair);
                    entity->m_componentsByType.insert(renderPair);
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
            std::shared_ptr<CRender> playerRenderComponent = std::dynamic_pointer_cast<CRender>(player->getComponentByType(Component::Type::RENDER));
            std::shared_ptr<CRender> enemyRenderComponent = std::dynamic_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            if (isCollidingAABB(playerRenderComponent, enemyRenderComponent))
            {
                player->isAlive = false;

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
    std::shared_ptr<CTransform> transformComponentForEntity = std::dynamic_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> collisionComponentForEntity = std::dynamic_pointer_cast<CCollision>(e->getComponentByType(Component::Type::COLLISION));
    std::shared_ptr<CRender> renderComponentForEntity = std::dynamic_pointer_cast<CRender>(e->getComponentByType(Component::Type::RENDER));

    collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->position.x <=
            renderComponentForEntity->renderBody.getRadius();

    collisionComponentForEntity->isCollidingRight = transformComponentForEntity->position.x >=
            WINDOW_WIDTH - renderComponentForEntity->renderBody.getRadius();

    collisionComponentForEntity->isCollidingUp = transformComponentForEntity->position.y <=
            renderComponentForEntity->renderBody.getRadius();

    collisionComponentForEntity->isCollidingDown = transformComponentForEntity->position.y >=
            WINDOW_HEIGHT - renderComponentForEntity->renderBody.getRadius();
}

void Engine::lifeSpanSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::LIFESPAN) && e->hasComponent(Component::Type::RENDER);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CLifespan> lifeSpanComponent = std::dynamic_pointer_cast<CLifespan> (e->getComponentByType(Component::Type::LIFESPAN));

        if (!lifeSpanComponent->isAlive())
        {
            e->isAlive = false;
        }
        else
        {
            std::shared_ptr<CRender> renderComponent = std::dynamic_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));
            sf::Color fillColor = renderComponent->renderBody.getFillColor();
            sf::Color outlineColor = renderComponent->renderBody.getOutlineColor();
            fillColor.a -= fillColor.a >= 2.55 ? 2.55 : 0;
            outlineColor.a -= outlineColor.a >= 2.55 ? 2.55 : 0;

            renderComponent->renderBody.setFillColor(fillColor);
            renderComponent->renderBody.setOutlineColor(outlineColor);

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
        std::shared_ptr<CTransform> transformComponentForEntity = std::dynamic_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CRender> renderComponentForEntity = std::dynamic_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));

        sf::CircleShape& drawable = renderComponentForEntity->renderBody;
        drawable.setPosition(transformComponentForEntity->position);
        drawable.rotate(2);

        m_window.draw(drawable);
    }
}

void Engine::spawnPlayer()
{
    std::shared_ptr<Entity>& e = m_entityManager.addEntity(Entity::Type::PLAYER);

    std::shared_ptr<CTransform> cTransform = std::make_shared<CTransform>();
    cTransform->position = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    cTransform->speed = sf::Vector2f(10.0f, 10.0f);
    std::pair transformPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::TRANSFORM, cTransform);
    e->m_componentsByType.insert(transformPair);

    std::shared_ptr<CCollision> cCollision = std::make_shared<CCollision>();
    std::pair collisionPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::COLLISION, cCollision);
    e->m_componentsByType.insert(collisionPair);

    std::shared_ptr<CUserInput> cUserInput = std::make_shared<CUserInput>();
    std::pair userInputPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::USER_INPUT, cUserInput);
    e->m_componentsByType.insert(userInputPair);

    std::shared_ptr<CRender> cRender = std::make_shared<CRender>();

    float radius = 30.0f;
    sf::CircleShape shape(radius, 8);
    shape.setOrigin(radius, radius);
    shape.setPosition(cTransform->position);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(3.0f);
    cRender->renderBody = shape;

    std::pair renderPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::RENDER, cRender);
    e->m_componentsByType.insert(renderPair);
}

void Engine::spawnEnemy()
{
    std::shared_ptr<Entity>& e = m_entityManager.addEntity(Entity::Type::ENEMY);

    int radius = std::experimental::randint(10, 50);
    int totalVertices = std::experimental::randint(3, 10);

    sf::Vector2f pos = sf::Vector2f(
            std::experimental::randint(radius, static_cast<int>(WINDOW_WIDTH-radius)),
            std::experimental::randint(radius, static_cast<int>(WINDOW_HEIGHT-radius))
            );

    std::shared_ptr<CTransform> cTransform = std::make_shared<CTransform>();
    cTransform->position = pos;
    cTransform->speed = sf::Vector2f(
            std::experimental::randint(1, 3),
            std::experimental::randint(1, 3)
    );
    std::pair transformPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::TRANSFORM, cTransform);
    e->m_componentsByType.insert(transformPair);

    std::shared_ptr<CCollision> cCollision = std::make_shared<CCollision>();
    std::pair collisionPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::COLLISION, cCollision);
    e->m_componentsByType.insert(collisionPair);

    std::shared_ptr<CRender> cRender = std::make_shared<CRender>();

    int r = std::experimental::randint(50, 255);
    int g = std::experimental::randint(50, 255);
    int b = std::experimental::randint(50, 255);
    sf::Color color = sf::Color(r, g, b, 255);

    sf::CircleShape shape(radius, totalVertices);
    shape.setOrigin(radius, radius);
    shape.setPosition(cTransform->position);
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(3.0f);
    cRender->renderBody = shape;

    std::pair renderPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::RENDER, cRender);
    e->m_componentsByType.insert(renderPair);
}

void Engine::spawnBullet(sf::Vector2f position)
{
    std::shared_ptr<Entity>& e = m_entityManager.addEntity(Entity::Type::BULLET);

    std::shared_ptr<CTransform> cTransform = std::make_shared<CTransform>();
    cTransform->position = sf::Vector2f(position);
    cTransform->speed = sf::Vector2f(10.0f, 10.0f);

    sf::Vector2f targetDestinationForBullet = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    float angleShot = atan2(targetDestinationForBullet.y  - cTransform->position.y,
            targetDestinationForBullet.x - cTransform->position.x);

    cTransform->speedDelta = sf::Vector2f(cos(angleShot) * 1.0f, sin(angleShot) * 1.0f);

    //cTransform->speed = sf::Vector2f(5.0f, 5.0f);
    std::pair transformPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::TRANSFORM, cTransform);
    e->m_componentsByType.insert(transformPair);

    std::shared_ptr<CCollision> cCollision = std::make_shared<CCollision>();
    std::pair collisionPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::COLLISION, cCollision);
    e->m_componentsByType.insert(collisionPair);

    std::shared_ptr<CLifespan> cLifespan = std::make_shared<CLifespan>(100);
    std::pair lifespanPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::LIFESPAN, cLifespan);
    e->m_componentsByType.insert(lifespanPair);

    std::shared_ptr<CRender> cRender = std::make_shared<CRender>();

    float radius = 10.0f;
    sf::CircleShape shape(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(cTransform->position);
    shape.setFillColor(sf::Color::White);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1.0f);
    cRender->renderBody = shape;

    std::pair renderPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::RENDER, cRender);
    e->m_componentsByType.insert(renderPair);
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
    if (!textureSprite.loadFromFile(texturePath)) {
        std::string msg = "Unable to open textureSprite '" + texturePath + "'\n";
        throw new std::runtime_error(msg);
    }
    backgroundSprite = sf::Sprite(textureSprite);
}

bool Engine::isCollidingAABB(
        const std::shared_ptr<CRender>& renderComponentForEntity,
        const std::shared_ptr<CRender>& renderComponentForEnemy)
{
    return renderComponentForEntity->renderBody.getGlobalBounds()
        .intersects(renderComponentForEnemy->renderBody.getGlobalBounds());
}

void Engine::drawText(sf::String text, sf::Color fillColour, uint8_t characterSize, sf::Vector2f position) {
    sf::Text sf_text = sf::Text(text, m_font);
    sf_text.setFillColor(fillColour);
    sf_text.setCharacterSize(characterSize); // in pixels, not points!
    sf_text.setPosition(position);

    // TODO parameterise us
    sf_text.setOutlineColor(sf::Color::Black);
    sf_text.setOutlineThickness(2.0f);
    sf_text.setLetterSpacing(3.0f);

    m_window.draw(sf_text);
}

void Engine::configureTextRendering()
{
    if (!m_font.loadFromFile(FONT_PATH))
    {
        std::string msg = "Failed to load font from font path: " + FONT_PATH;
        throw new std::runtime_error(msg);
    }
}