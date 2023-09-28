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
                handleMouseHover();
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                handleMouseClick();
                break;
            }
            default:
                break;
        }
    }
}

void MenuUserInputSystem::handleMouseClick()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    if (exitTextualButton.getGlobalBounds().contains(mousePosF))
    {
        gameEngine.window.close();
        return;
    }

    if (startGameTextualButton.getGlobalBounds().contains(mousePosF))
    {
        const std::shared_ptr<GameplayScene>& nextScene = std::make_shared<GameplayScene>(gameEngine);
        gameEngine.changeScene(Scene::Type::GAMEPLAY_SCENE, nextScene);
        return;
    }

}
void MenuUserInputSystem::handleMouseHover()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    if (startGameTextualButton.getGlobalBounds().contains(mousePosF))
    {
        onHover(startGameTextualButton, sf::Color{0, 255, 255}, sf::Cursor::Hand);
    }
    else if (exitTextualButton.getGlobalBounds().contains(mousePosF))
    {
        onHover(exitTextualButton, sf::Color{0, 255, 255}, sf::Cursor::Hand);
    }
    else
    {
        onHover(startGameTextualButton, sf::Color{255, 255, 255}, sf::Cursor::Arrow);
        onHover(exitTextualButton, sf::Color{255, 255, 255}, sf::Cursor::Arrow);
    }
}

void MenuUserInputSystem::onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover)
{
    if (cursor.loadFromSystem(cursorTypeOnHover))
    {
        gameEngine.window.setMouseCursor(cursor);
    }
    text.setFillColor(color);
}

bool MenuUserInputSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}
