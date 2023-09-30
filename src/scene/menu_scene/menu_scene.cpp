#include "scene/menu_scene/menu_scene.h"

MenuScene::MenuScene(GameEngine& gameEngine) : Scene(gameEngine)
{
    registerCursorActionType(sf::Event::EventType::MouseEntered, Action::Type::CURSOR_MOVE);
    registerCursorActionType(sf::Event::EventType::MouseMoved, Action::Type::CURSOR_MOVE);

    registerActionType(sf::Keyboard::Key::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::Down, Action::Type::MOVE_DOWN);
    registerActionType(CursorButton::CURSOR_LEFT, Action::Type::CURSOR_SELECT);
    registerActionType(sf::Keyboard::Key::Enter, Action::Type::SELECT);

    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    gameTitleTextPair = createTextElementPair("Primitive Wars", 96, TITLE_TEXT_COLOUR, sf::Color::Black, 3.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 250));
    startGameTextualButtonPair = createTextElementPair("Start Game", 72, BUTTON_DEFAULT_COLOR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));
    exitTextualButtonPair = createTextElementPair("Quit Game", 72, BUTTON_DEFAULT_COLOR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 150));

    m_audioManager->playMusic(static_cast<uint8_t>(Scene::Type::MENU_SCENE), 30.0f, true);
}

void MenuScene::update()
{
    if (currentSelectItem == 0)
    {
        exitTextualButtonPair.second.setFillColor(exitTextualButtonPair.first);
        startGameTextualButtonPair.second.setFillColor(BUTTON_HIGHLIGHT_COLOR);
    }
    else if (currentSelectItem == 1)
    {
        startGameTextualButtonPair.second.setFillColor(startGameTextualButtonPair.first);
        exitTextualButtonPair.second.setFillColor(BUTTON_HIGHLIGHT_COLOR);
    }
}

void MenuScene::render()
{
    gameEngine.window.clear(BACKGROUND_COLOR);

    gameEngine.window.draw(gameTitleTextPair.second);
    gameEngine.window.draw(startGameTextualButtonPair.second);
    gameEngine.window.draw(exitTextualButtonPair.second);

    gameEngine.window.display();
}

void MenuScene::performAction(Action& action)
{
    switch (action.getType())
    {
        case Action::Type::MOVE_DOWN:
        case Action::Type::MOVE_UP:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem + 1) % TOTAL_TEXT_BUTTONS;
            }
            break;
        case Action::Type::CURSOR_MOVE:
        {
            handleMouseHover();
            break;
        }
        case Action::Type::SELECT:
        {
            if (currentSelectItem == 0)
            {
                changeToLevelSelectScene();
            }
            else if (currentSelectItem == 1)
            {
                gameEngine.window.close();
                return;
            }
        }
        case Action::Type::CURSOR_SELECT:
        {
            if (action.getMode() == Action::Mode::PRESS)
            {
                handleMouseClick();
            }
            break;
        }
        default:
            break;
    }
}

void MenuScene::changeToLevelSelectScene()
{
    const std::shared_ptr<LevelSelectScene>& nextScene = std::make_shared<LevelSelectScene>(gameEngine,
            LevelSelectScene::LevelClearStatus(false, false, false));
    gameEngine.changeScene(Type::LEVEL_SELECT_SCENE, nextScene);
}

void MenuScene::handleMouseClick()
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
        changeToLevelSelectScene();
        return;
    }

}
void MenuScene::handleMouseHover()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    auto& [originalStartTextButtonColor, startTextButton] = startGameTextualButtonPair;
    if (startTextButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = -1;
        onHover(startTextButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    auto& [originalExitTextButtonColor, exitTextButton] = exitTextualButtonPair;
    if (exitTextButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = -1;
        onHover(exitTextButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    onHover(startTextButton, originalStartTextButtonColor, sf::Cursor::Arrow);
    onHover(exitTextButton, originalExitTextButtonColor, sf::Cursor::Arrow);
}

void MenuScene::onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover)
{
    if (cursor.loadFromSystem(cursorTypeOnHover))
    {
        gameEngine.window.setMouseCursor(cursor);
    }
    text.setFillColor(color);
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
std::pair<sf::Color, sf::Text> MenuScene::createTextElementPair(const std::string& value, uint16_t characterSize,
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
