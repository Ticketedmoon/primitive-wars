#include "scene/menu_scene/system/menu_user_input_system.h"

MenuUserInputSystem::MenuUserInputSystem(GameEngine& gameEngine, EntityManager& entityManager,
        sf::Text& startGameTextualButton, sf::Text& exitTextualButton)
        : gameEngine(gameEngine), m_entityManager(entityManager),
        startGameTextualButtonPair{startGameTextualButton.getFillColor(), startGameTextualButton},
        exitTextualButtonPair{exitTextualButton.getFillColor(), exitTextualButton}
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

    auto& [exitColor, exitTextButton] = exitTextualButtonPair;
    if (exitTextButton.getGlobalBounds().contains(mousePosF))
    {
        gameEngine.window.close();
        return;
    }

    auto& [startColor, startTextButton] = startGameTextualButtonPair;
    if (startTextButton.getGlobalBounds().contains(mousePosF))
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

    auto& [originalStartTextButtonColor, startTextButton] = startGameTextualButtonPair;
    if (startTextButton.getGlobalBounds().contains(mousePosF))
    {
        onHover(startTextButton, sf::Color{0, 255, 255}, sf::Cursor::Hand);
        return;
    }

    auto& [originalExitTextButtonColor, exitTextButton] = exitTextualButtonPair;
    if (exitTextButton.getGlobalBounds().contains(mousePosF))
    {
        onHover(exitTextButton, sf::Color{0, 255, 255}, sf::Cursor::Hand);
        return;
    }

    onHover(startTextButton, originalStartTextButtonColor, sf::Cursor::Arrow);
    onHover(exitTextButton, originalExitTextButtonColor, sf::Cursor::Arrow);
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
