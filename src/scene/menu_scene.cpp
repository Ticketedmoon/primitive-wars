#include "scene/menu_scene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    startGameTextualButton.setFont(m_font);
    exitTextualButton.setFont(m_font);

    uint32_t buttonWidth = WINDOW_WIDTH / 4;
    uint32_t buttonHeight = WINDOW_HEIGHT / 6;

    startGameTextualButton.setOrigin(buttonWidth/2, buttonHeight/2);
    startGameTextualButton.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 - 100);
    startGameTextualButton.setString("Start Game");
    startGameTextualButton.setStyle(sf::Text::Bold);
    startGameTextualButton.setCharacterSize(72);
    startGameTextualButton.setOutlineColor(sf::Color::Black);
    startGameTextualButton.setOutlineThickness(3.0f);

    exitTextualButton.setOrigin(buttonWidth/2, buttonHeight/2);
    exitTextualButton.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2 + 100);
    exitTextualButton.setString("Quit Game");
    exitTextualButton.setStyle(sf::Text::Bold);
    exitTextualButton.setCharacterSize(72);
    startGameTextualButton.setOutlineColor(sf::Color::Black);
    startGameTextualButton.setOutlineThickness(3.0f);

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

    gameEngine.window.draw(startGameTextualButton);
    gameEngine.window.draw(exitTextualButton);

    gameEngine.window.display();
}

void MenuScene::performAction()
{

}