#pragma once

#ifndef PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H
#define PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H

#include <SFML/Graphics/RectangleShape.hpp>

#include "game_engine.h"
#include "scene.h"

class LevelSelectScene : public Scene
{
    public:
        explicit LevelSelectScene(GameEngine& gameEngine, LevelClearStatus updatedLevelClearStatus);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void handleMouseHover();
        void handleMouseClick();
        void handleButtonSelect();
        void onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover);

        void createTextElements(const LevelClearStatus& updatedLevelClearStatus);
        void changeToLevelWithProperties(Difficulty difficulty, float enemySpawnRateSeconds, float enemySpeed);

        std::pair<sf::Color, sf::Text> createTextElementPair(const std::string& value, uint16_t characterSize,
                sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position);
        void registerActions();

    private:
        static inline const std::string FONT_PATH = "resources/fonts/calibri.ttf";
        static inline const sf::Color& BACKGROUND_COLOR = sf::Color{20, 20, 20, 255};
        static inline const uint8_t TOTAL_TEXT_BUTTONS = 3;
        static inline const sf::Color& LEVEL_ONE_TEXT_COLOR = sf::Color::Green;
        static inline const sf::Color& LEVEL_TWO_TEXT_COLOR = sf::Color::Blue;
        static inline const sf::Color& LEVEL_THREE_TEXT_COLOR = sf::Color::Red;
        static inline const sf::Color& BUTTON_HIGHLIGHT_COLOR = sf::Color{255, 255, 255};
        static inline const uint8_t BUTTON_FONT_SIZE = 64;

        static inline LevelClearStatus levelClearStatus;

        sf::Font m_font;
        uint8_t currentSelectItem = 0;
        std::pair<sf::Color, sf::Text> levelOneTextButton;
        std::pair<sf::Color, sf::Text> levelTwoTextButton;
        std::pair<sf::Color, sf::Text> levelThreeTextButton;
        sf::Cursor cursor;
};

#endif //PRIMITIVE_WARS_LEVEL_SELECT_SCENE_H
