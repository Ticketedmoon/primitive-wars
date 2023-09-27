#include "user_input_system.h"

UserInputSystem::UserInputSystem(EntityManager& entityManager, sf::RenderWindow& renderWindow, sf::Clock& worldClock,
        GuiProperties& guiProperties) : m_entityManager(entityManager), m_renderWindow(renderWindow),
        m_worldClock(worldClock), m_guiProperties(guiProperties)
{

}

void UserInputSystem::execute()
{
    sf::Event event{};

    while (m_renderWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_renderWindow.close();
        }

        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Key::P)
            {
                m_guiProperties.hasPaused = !m_guiProperties.hasPaused;
            }
        }

        if (m_guiProperties.hasPaused)
        {
            return;
        }

        std::ranges::filter_view view = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
            return e->hasComponent(Component::Type::USER_INPUT);
        });
        std::vector<std::shared_ptr<Entity>> entitiesToUpdate = std::vector(view.begin(), view.end());
        for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
        {
            std::shared_ptr<CUserInput> userInputComponentForEntity = std::static_pointer_cast<CUserInput>(
                    e->getComponentByType(Component::Type::USER_INPUT));

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
                userInputComponentForEntity->mouseLeftClicked = event.mouseButton.button == sf::Mouse::Left;

                if (m_guiProperties.specialAttackCoolDownSeconds <= m_worldClock.getElapsedTime().asSeconds())
                {
                    userInputComponentForEntity->mouseRightClicked = event.mouseButton.button == sf::Mouse::Right;
                }

                userInputComponentForEntity->mouseClickPosition = m_renderWindow
                        .mapPixelToCoords(sf::Mouse::getPosition(m_renderWindow));
            }
        }
    }
}
