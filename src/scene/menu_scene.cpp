#include "scene/menu_scene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    gameTitleText.setFont(m_font);
    startGameTextualButton.setFont(m_font);
    exitTextualButton.setFont(m_font);

    gameTitleText.setString("Primitive Wars");
    gameTitleText.setStyle(sf::Text::Bold);
    gameTitleText.setCharacterSize(96);
    gameTitleText.setFillColor(sf::Color::Yellow);
    gameTitleText.setOutlineColor(sf::Color::Black);
    gameTitleText.setOutlineThickness(3.0f);
    gameTitleText.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 250);
    gameTitleText.setOrigin(gameTitleText.getGlobalBounds().width/2, gameTitleText.getGlobalBounds().height/2);

    startGameTextualButton.setString("Start Game");
    startGameTextualButton.setStyle(sf::Text::Bold);
    startGameTextualButton.setCharacterSize(72);
    startGameTextualButton.setOutlineColor(sf::Color::Black);
    startGameTextualButton.setOutlineThickness(3.0f);
    startGameTextualButton.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 50);
    startGameTextualButton.setOrigin(startGameTextualButton.getGlobalBounds().width/2, startGameTextualButton.getGlobalBounds().height/2);

    exitTextualButton.setString("Quit Game");
    exitTextualButton.setStyle(sf::Text::Bold);
    exitTextualButton.setCharacterSize(72);
    exitTextualButton.setOutlineColor(sf::Color::Black);
    exitTextualButton.setOutlineThickness(3.0f);
    exitTextualButton.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 150);
    exitTextualButton.setOrigin(exitTextualButton.getGlobalBounds().width/2, exitTextualButton.getGlobalBounds().height/2);

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

void MenuScene::performAction()
{

}