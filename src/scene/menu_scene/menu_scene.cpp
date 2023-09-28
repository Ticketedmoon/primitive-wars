#include "scene/menu_scene/menu_scene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    createTextElement(gameTitleText, "Primitive Wars", 96, sf::Color::Yellow, sf::Color::Black, 3.0f, sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 250));
    createTextElement(startGameTextualButton, "Start Game", 72, sf::Color::White, sf::Color::Black, 3.0f, sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 50));
    createTextElement(exitTextualButton, "Quit Game", 72, sf::Color::White, sf::Color::Black, 3.0f, sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 150));

    m_systemManager.registerSystem(
            std::make_shared<MenuUserInputSystem>(gameEngine, m_entityManager, startGameTextualButton, exitTextualButton), SystemManager::SystemType::UPDATE);

}

void MenuScene::update()
{
    m_systemManager.update(gameEngine.gameProperties);
}

void MenuScene::render()
{
    gameEngine.window.clear(sf::Color{0, 150, 20, 255});

    gameEngine.window.draw(gameTitleText);
    gameEngine.window.draw(startGameTextualButton);
    gameEngine.window.draw(exitTextualButton);

    gameEngine.window.display();
}

void MenuScene::performAction(Action& action)
{

}

void MenuScene::createTextElement(sf::Text& text, const std::string& value, const uint16_t characterSize,
        const sf::Color fillColor, const sf::Color outlineColor, const float outlineThickness,
        const sf::Vector2f position)
{
    text.setFont(m_font);
    text.setString(value);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(characterSize);
    text.setFillColor(fillColor);
    text.setOutlineColor(outlineColor);
    text.setOutlineThickness(outlineThickness);
    text.setPosition(position);
    text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
}
