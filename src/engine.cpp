#include "../include/engine.h"

Engine::Engine()
{
    createGameWindow();
}

void Engine::startGameLoop()
{
    sf::Clock deltaClock;
    createPlayer();

    while (m_window.isOpen())
    {
        deltaClock.restart();

        listenForEvents();
        update();
        render();
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

    // Do something
    renderSystem();

    m_window.display();
}

void Engine::renderSystem()
{
    std::ranges::filter_view view = m_entityManager.getEntities() | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::RENDER) && e->hasComponent(Component::Type::TRANSFORM);
    });
    std::vector<std::shared_ptr<Entity>> entitiesToRender = std::vector(view.begin(), view.end());
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CTransform> transformComponent = std::dynamic_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CRender> renderComponent = std::dynamic_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));

        sf::CircleShape shape(renderComponent->radius);
        shape.setOrigin(renderComponent->radius, renderComponent->radius);
        shape.setPosition(transformComponent->position);
        shape.setFillColor(renderComponent->color);
        m_window.draw(shape);
    }
}

void Engine::createPlayer()
{
    std::shared_ptr<Entity>& e = m_entityManager.addEntity(Entity::Type::PLAYER);

    std::shared_ptr<CTransform> cTransform = std::make_shared<CTransform>();
    cTransform->position = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    std::pair transformPair = std::make_pair<Component::Type, std::shared_ptr<Component>>(Component::Type::TRANSFORM, cTransform);
    e->m_componentsByType.insert(transformPair);

    std::shared_ptr<CRender> cRender = std::make_shared<CRender>();
    cRender->radius = 100.0f;
    cRender->color = sf::Color::Red;
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
}