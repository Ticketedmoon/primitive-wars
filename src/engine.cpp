#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
}

void Engine::startGameLoop()
{
    createPlayer();

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
}

void Engine::render()
{
    m_window.clear();
    m_window.draw(backgroundSprite);

    // Do something
    enemySpawnSystem();
    userInputSystem();
    collisionSystem();
    transformSystem();
    renderSystem();

    m_window.display();
}

void Engine::transformSystem()
{
    std::ranges::filter_view entitiesFiltered = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToRender = std::vector(entitiesFiltered.begin(), entitiesFiltered.end());
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CTransform> transformComponent = std::dynamic_pointer_cast<CTransform>(e->getComponentByType(
                Component::Type::TRANSFORM));
        std::shared_ptr<CCollision> collisionComponent = std::dynamic_pointer_cast<CCollision>(e->getComponentByType(
                Component::Type::COLLISION));

        if (e->hasComponent(Component::Type::USER_INPUT))
        {
            std::shared_ptr<CUserInput> userInputComponent = std::dynamic_pointer_cast<CUserInput>(e->getComponentByType(
                    Component::Type::USER_INPUT));

            if (!collisionComponent->isCollidingLeft)
            {
                transformComponent->position.x -= userInputComponent->movingLeft ? transformComponent->speed.x : 0;;
            }
            if (!collisionComponent->isCollidingRight)
            {
                transformComponent->position.x += userInputComponent->movingRight ? transformComponent->speed.x : 0;
            }
            if (!collisionComponent->isCollidingUp)
            {
                transformComponent->position.y -= userInputComponent->movingUp ? transformComponent->speed.y : 0;
            }
            if (!collisionComponent->isCollidingDown)
            {
                transformComponent->position.y += userInputComponent->movingDown ? transformComponent->speed.y : 0;
            }
        }
        else
        {
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
}

void Engine::userInputSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::USER_INPUT);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToRender = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CUserInput> userInputComponentForEntity = std::dynamic_pointer_cast<CUserInput> (e->getComponentByType(Component::Type::USER_INPUT));
        userInputComponentForEntity->movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        userInputComponentForEntity->movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        userInputComponentForEntity->movingUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        userInputComponentForEntity->movingDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
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
    std::ranges::filter_view entitiesFiltered = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::RENDER);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToRender = std::vector(entitiesFiltered.begin(), entitiesFiltered.end());
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CTransform> transformComponentForEntity = std::dynamic_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CCollision> collisionComponentForEntity = std::dynamic_pointer_cast<CCollision> (e->getComponentByType(Component::Type::COLLISION));
        std::shared_ptr<CRender> renderComponentForEntity = std::dynamic_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));

        collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->position.x <= renderComponentForEntity->renderBody.getRadius();
        collisionComponentForEntity->isCollidingRight = transformComponentForEntity->position.x >= WINDOW_WIDTH - renderComponentForEntity->renderBody.getRadius();
        collisionComponentForEntity->isCollidingUp = transformComponentForEntity->position.y <= renderComponentForEntity->renderBody.getRadius();
        collisionComponentForEntity->isCollidingDown = transformComponentForEntity->position.y >= WINDOW_HEIGHT - renderComponentForEntity->renderBody.getRadius();
    }
}

void Engine::renderSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::RENDER) && e->hasComponent(Component::Type::TRANSFORM);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToRender = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CTransform> transformComponentForEntity = std::dynamic_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CRender> renderComponentForEntity = std::dynamic_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));

        sf::CircleShape& drawable = renderComponentForEntity->renderBody;
        drawable.setPosition(transformComponentForEntity->position);
        drawable.rotate(3);

        m_window.draw(drawable);
    }
}

void Engine::createPlayer()
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

    int radius = std::experimental::randint(0, 50);
    int totalVertices = std::experimental::randint(0, 32);

    sf::Vector2f pos = sf::Vector2f(
            std::experimental::randint(radius, static_cast<int>(WINDOW_WIDTH-radius)),
            std::experimental::randint(radius, static_cast<int>(WINDOW_HEIGHT-radius))
            );

    std::shared_ptr<CTransform> cTransform = std::make_shared<CTransform>();
    cTransform->position = pos;
    cTransform->speed = sf::Vector2f(
            std::experimental::randint(1, 5),
            std::experimental::randint(1, 5)
    );
    std::pair transformPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::TRANSFORM, cTransform);
    e->m_componentsByType.insert(transformPair);

    std::shared_ptr<CCollision> cCollision = std::make_shared<CCollision>();
    std::pair collisionPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::COLLISION, cCollision);
    e->m_componentsByType.insert(collisionPair);

    std::shared_ptr<CRender> cRender = std::make_shared<CRender>();

    int r = std::experimental::randint(0, 255);
    int g = std::experimental::randint(0, 255);
    int b = std::experimental::randint(0, 255);
    sf::Color color = sf::Color(r, g, b, 255);

    sf::CircleShape shape(radius, totalVertices);
    shape.setOrigin(radius, radius);
    shape.setPosition(cTransform->position);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(color);
    shape.setOutlineThickness(3.0f);
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
