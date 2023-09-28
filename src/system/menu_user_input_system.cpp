#include "menu_user_input_system.h"

MenuUserInputSystem::MenuUserInputSystem(GameEngine& gameEngine, EntityManager& entityManager,
        sf::Text& startGameTextualButton, sf::Text& exitTextualButton)
        : gameEngine(gameEngine), m_entityManager(entityManager), startGameTextualButton(startGameTextualButton),
        exitTextualButton(exitTextualButton)
{

}

void MenuUserInputSystem::execute()
{
    sf::Event event{};

    while (gameEngine.window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                gameEngine.window.close();
                break;
            case sf::Event::MouseMoved:
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
                sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));
                
                if (startGameTextualButton.getGlobalBounds().contains(mousePosF))
                {
                    if (cursor.loadFromSystem(sf::Cursor::Hand))
                    {
                        gameEngine.window.setMouseCursor(cursor);
                    }
                    startGameTextualButton.setFillColor(sf::Color(0, 255, 255));
                }
                else if (exitTextualButton.getGlobalBounds().contains(mousePosF))
                {
                    if (cursor.loadFromSystem(sf::Cursor::Hand))
                    {
                        gameEngine.window.setMouseCursor(cursor);
                    }
                    exitTextualButton.setFillColor(sf::Color(250, 20, 20));
                }
                else
                {
                    if (cursor.loadFromSystem(sf::Cursor::Arrow))
                    {
                        gameEngine.window.setMouseCursor(cursor);
                    }
                    // FIXME
                    startGameTextualButton.setFillColor(sf::Color(255, 255, 255));
                    exitTextualButton.setFillColor(sf::Color(255, 255, 255));
                }
            }
                break;
            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
                sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

                if (exitTextualButton.getGlobalBounds().contains(mousePosF))
                {
                    gameEngine.window.close();
                    break;
                }
                if (startGameTextualButton.getGlobalBounds().contains(mousePosF))
                {
                    const std::shared_ptr<GameplayScene>& nextScene = std::make_shared<GameplayScene>(gameEngine);
                    gameEngine.changeScene(Scene::Type::GAMEPLAY_SCENE, nextScene);
                }
            }
            default:
                break;
        }
    }
}

bool MenuUserInputSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}
