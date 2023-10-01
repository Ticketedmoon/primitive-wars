#include "scene/level_select_scene/level_select_scene.h"

LevelSelectScene::LevelSelectScene(GameEngine& gameEngine, LevelClearStatus updatedLevelClearStatus) : Scene(gameEngine)
{
    registerCursorActionType(sf::Event::EventType::MouseEntered, Action::Type::CURSOR_MOVE);
    registerCursorActionType(sf::Event::EventType::MouseMoved, Action::Type::CURSOR_MOVE);

    registerActionType(sf::Keyboard::Key::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::Down, Action::Type::MOVE_DOWN);
    registerActionType(CursorButton::CURSOR_LEFT, Action::Type::CURSOR_SELECT);
    registerActionType(sf::Keyboard::Key::Enter, Action::Type::SELECT);

    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    createTextElements(updatedLevelClearStatus);

    if (!m_audioManager->isMusicPlaying())
    {
        m_audioManager->playMusic(static_cast<uint8_t>(Scene::Type::LEVEL_SELECT_SCENE), 30.0f, true);
    }
}

void LevelSelectScene::update()
{
    if (currentSelectItem == 0)
    {
        levelOneTextButton.second.setFillColor(BUTTON_HIGHLIGHT_COLOR);
        levelTwoTextButton.second.setFillColor(levelTwoTextButton.first);
        levelThreeTextButton.second.setFillColor(levelThreeTextButton.first);
    }
    else if (currentSelectItem == 1)
    {
        levelOneTextButton.second.setFillColor(levelOneTextButton.first);
        levelTwoTextButton.second.setFillColor(BUTTON_HIGHLIGHT_COLOR);
        levelThreeTextButton.second.setFillColor(levelThreeTextButton.first);
    }
    else if (currentSelectItem == 2)
    {
        levelOneTextButton.second.setFillColor(levelOneTextButton.first);
        levelTwoTextButton.second.setFillColor(levelTwoTextButton.first);
        levelThreeTextButton.second.setFillColor(BUTTON_HIGHLIGHT_COLOR);
    }
}

void LevelSelectScene::render()
{
    gameEngine.window.clear(BACKGROUND_COLOR);

    gameEngine.window.draw(levelOneTextButton.second);
    gameEngine.window.draw(levelTwoTextButton.second);
    gameEngine.window.draw(levelThreeTextButton.second);

    gameEngine.window.display();
}

void LevelSelectScene::performAction(Action& action)
{
    switch (action.getType())
    {
        case Action::Type::MOVE_DOWN:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem + 1) % TOTAL_TEXT_BUTTONS;
            }
            break;
        case Action::Type::MOVE_UP:
            if (action.getMode() == Action::Mode::PRESS)
            {
                currentSelectItem = (currentSelectItem == 0 ? TOTAL_TEXT_BUTTONS-1 : currentSelectItem - 1) % TOTAL_TEXT_BUTTONS;
            }
            break;
        case Action::Type::CURSOR_MOVE:
        {
            handleMouseHover();
            break;
        }
        case Action::Type::SELECT:
        {
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }
            handleButtonSelect();
        }
        case Action::Type::CURSOR_SELECT:
        {
            if (action.getMode() == Action::Mode::RELEASE)
            {
                return;
            }
            handleMouseClick();
            break;
        }
        default:
            break;
    }
}

void LevelSelectScene::handleButtonSelect()
{
    if (currentSelectItem == 0)
    {
        changeToLevelWithProperties(Difficulty::EASY, 1.0f, 1.0f);
    }
    else if (currentSelectItem == 1)
    {
        changeToLevelWithProperties(Difficulty::MEDIUM, 0.5f, 1.25f);
    }
    else if (currentSelectItem == 2)
    {
        changeToLevelWithProperties(Difficulty::HARD, 0.25f, 1.5f);
    }
}

void LevelSelectScene::handleMouseClick()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    if (levelOneTextButton.second.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 0;
        handleButtonSelect();
        return;
    }

    if (levelTwoTextButton.second.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 1;
        handleButtonSelect();
        return;
    }

    if (levelThreeTextButton.second.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = 2;
        handleButtonSelect();
        return;
    }

}

void LevelSelectScene::handleMouseHover()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(gameEngine.window);
    sf::Vector2f mousePosF(static_cast<float>( mousePos.x ), static_cast<float>( mousePos.y ));

    auto& [originalLevelOneTextButtonColor, levelOneButton] = levelOneTextButton;
    if (levelOneButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = -1;
        onHover(levelOneButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    auto& [originalLevelTwoTextButtonColor, levelTwoButton] = levelTwoTextButton;
    if (levelTwoButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = -1;
        onHover(levelTwoButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    auto& [originalLevelThreeTextButtonColor, levelThreeButton] = levelThreeTextButton;
    if (levelThreeButton.getGlobalBounds().contains(mousePosF))
    {
        currentSelectItem = -1;
        onHover(levelThreeButton, BUTTON_HIGHLIGHT_COLOR, sf::Cursor::Hand);
        return;
    }

    onHover(levelOneButton, originalLevelOneTextButtonColor, sf::Cursor::Arrow);
    onHover(levelTwoButton, originalLevelTwoTextButtonColor, sf::Cursor::Arrow);
    onHover(levelThreeButton, originalLevelThreeTextButtonColor, sf::Cursor::Arrow);
}

void LevelSelectScene::onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover)
{
    if (cursor.loadFromSystem(cursorTypeOnHover))
    {
        gameEngine.window.setMouseCursor(cursor);
    }
    text.setFillColor(color);
}

void LevelSelectScene::changeToLevelWithProperties(Difficulty difficulty, float enemySpawnRateSeconds, float enemySpeed)
{
    GameProperties gameProperties;
    gameProperties.difficulty = difficulty;
    gameProperties.enemySpawnRateSeconds = enemySpawnRateSeconds;
    gameProperties.enemySpeed = enemySpeed;
    const std::shared_ptr<GameplayScene>& nextScene = std::make_shared<GameplayScene>(gameEngine, gameProperties);
    const Scene::Type sceneType = difficulty == Difficulty::EASY
            ? Scene::Type::LEVEL_ONE_GAMEPLAY_SCENE
            : difficulty == Difficulty::MEDIUM
                ? Scene::Type::LEVEL_TWO_GAMEPLAY_SCENE
                : Scene::Type::LEVEL_THREE_GAMEPLAY_SCENE;
    gameEngine.changeScene(sceneType, nextScene);
}

void LevelSelectScene::createTextElements(const LevelSelectScene::LevelClearStatus& updatedLevelClearStatus)
{
    levelClearStatus.levelOneCleared = (updatedLevelClearStatus.levelOneCleared || levelClearStatus.levelOneCleared);
    levelClearStatus.levelTwoCleared = (updatedLevelClearStatus.levelTwoCleared || levelClearStatus.levelTwoCleared);
    levelClearStatus.levelThreeCleared = (updatedLevelClearStatus.levelThreeCleared || levelClearStatus.levelThreeCleared);

    std::string levelOneTextValue = levelClearStatus.levelOneCleared ? "Level 1 [CLEAR]" : "Level 1 [X]";
    std::string levelTwoTextValue = levelClearStatus.levelTwoCleared ? "Level 2 [CLEAR]" : "Level 2 [X]";
    std::string levelThreeTextValue = levelClearStatus.levelThreeCleared ? "Level 3 [CLEAR]" : "Level 3 [X]";

    levelOneTextButton = createTextElementPair(levelOneTextValue, BUTTON_FONT_SIZE, LEVEL_ONE_TEXT_COLOR, sf::Color::Black, 3.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 250));
    levelTwoTextButton = createTextElementPair(levelTwoTextValue, BUTTON_FONT_SIZE, LEVEL_TWO_TEXT_COLOR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));
    levelThreeTextButton = createTextElementPair(levelThreeTextValue, BUTTON_FONT_SIZE, LEVEL_THREE_TEXT_COLOR, sf::Color::Black, 2.0f,
            sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 150));
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
std::pair<sf::Color, sf::Text> LevelSelectScene::createTextElementPair(const std::string& value, uint16_t characterSize,
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
