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
                    startGameTextualButton.setFillColor(sf::Color(250, 20, 20));
                }
                else
                {
                    // FIXME
                    startGameTextualButton.setFillColor(sf::Color(255, 255, 255));
                }
                
                if (exitTextualButton.getGlobalBounds().contains(mousePosF))
                {
                    exitTextualButton.setFillColor(sf::Color(250, 20, 20));
                }
                else
                {
                    // FIXME
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
