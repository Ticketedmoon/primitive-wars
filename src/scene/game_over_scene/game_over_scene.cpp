#include "scene/game_over_scene/game_over_scene.h"

GameOverScene::GameOverScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    registerCursorActionType(sf::Event::EventType::MouseEntered, Action::Type::CURSOR_MOVE);
    registerCursorActionType(sf::Event::EventType::MouseMoved, Action::Type::CURSOR_MOVE);

    registerActionType(sf::Keyboard::Key::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::Down, Action::Type::MOVE_DOWN);
    registerActionType(CursorButton::CURSOR_LEFT, Action::Type::CURSOR_SELECT);
    registerActionType(sf::Keyboard::Key::Enter, Action::Type::SELECT);

    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    gameOverText = createTextElementPair("Game Over!", 96, TITLE_TEXT_COLOUR, sf::Color::Black, 3.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 250));

    menuReturnTextButton = createTextElementPair("Game Statistics:", 72, BUTTON_DEFAULT_COLOR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));

    menuReturnTextButton = createTextElementPair("Return to Menu", 72, BUTTON_DEFAULT_COLOR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));

    m_audioManager->playSound(AudioManager::AudioType::GAME_OVER, 30.0f);
}

void GameOverScene::update()
{
    // NOOP
}

void GameOverScene::render()
{
    gameEngine.window.clear(BACKGROUND_COLOR);

    gameEngine.window.draw(gameOverText.second);
    gameEngine.window.draw(menuReturnTextButton.second);
    gameEngine.window.draw(gameStatsText.second);

    gameEngine.window.display();
}

void GameOverScene::performAction(Action& action)
{
    switch (action.getType())
    {
        case Action::Type::CURSOR_MOVE:
        {
            handleMouseHover();
            break;
        }
        case Action::Type::SELECT:
        {
            changeToMenuScene();
        }
        case Action::Type::CURSOR_SELECT:
        {
            handleMouseClick();
            break;
        }
        default:
            break;
    }
}

void GameOverScene::handleMouseClick()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    auto& [exitColor, exitTextButton] = gameStatsText;
    if (exitTextButton.getGlobalBounds().contains(mousePosF))
    {
        gameEngine.window.close();
        return;
    }

    auto& [startColor, startTextButton] = menuReturnTextButton;
    if (startTextButton.getGlobalBounds().contains(mousePosF))
    {
        changeToMenuScene();
        return;
    }

}
void GameOverScene::handleMouseHover()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    auto& [originalStartTextButtonColor, startTextButton] = menuReturnTextButton;
    if (startTextButton.getGlobalBounds().contains(mousePosF))
    {
        onHover(startTextButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    auto& [originalExitTextButtonColor, exitTextButton] = gameStatsText;
    if (exitTextButton.getGlobalBounds().contains(mousePosF))
    {
        onHover(exitTextButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    onHover(startTextButton, originalStartTextButtonColor, sf::Cursor::Arrow);
    onHover(exitTextButton, originalExitTextButtonColor, sf::Cursor::Arrow);
}

void GameOverScene::onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover)
{
    if (cursor.loadFromSystem(cursorTypeOnHover))
    {
        gameEngine.window.setMouseCursor(cursor);
    }
    text.setFillColor(color);
}

void GameOverScene::changeToMenuScene()
{
    const std::shared_ptr<MenuScene>& nextScene = std::make_shared<MenuScene>(gameEngine);
    gameEngine.changeScene(Type::MENU_SCENE, nextScene);
}

/**
 * Creates a text element pairing from the original text color to the sf::Text object.
 * This is useful when we want to update the colour on hover and then back to the original colour afterwards.
 *
 * @param value The text value
 * @param characterSize The character size
 * @param fillColor The fill Color
 * @param outlineColor The outline Color
 * @param outlineThickness The outline thickness
 * @param position The position of the text
 * @return A text element pairing from the original text color to the sf::Text object.
 */
std::pair<sf::Color, sf::Text> GameOverScene::createTextElementPair(const std::string& value, uint16_t characterSize,
        sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position)
{
    sf::Text text;
    text.setPosition(position);
    text.setFont(m_font);
    text.setString(value);
    text.setStyle(sf::Text::Bold);
    text.setCharacterSize(characterSize);
    text.setFillColor(fillColor);
    text.setOutlineColor(outlineColor);
    text.setOutlineThickness(outlineThickness);
    text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
    return std::make_pair(text.getFillColor(), text);
}
