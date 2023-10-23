#pragma once

#ifndef PRIMITIVE_WARS_MENU_SCENE_H
#define PRIMITIVE_WARS_MENU_SCENE_H

#include <SFML/Graphics/RectangleShape.hpp>

#include "level_select_scene/level_select_scene.h"
#include "game_engine.h"
#include "scene.h"

class MenuScene : public Scene
{
    public:
        explicit MenuScene(GameEngine& gameEngine, sf::Clock& deltaClock);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void handleMouseHover();
        void handleMouseClick();
        void onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover);
        void changeToLevelSelectScene();

        std::pair<sf::Color, sf::Text> createTextElementPair(const std::string& value, uint16_t characterSize,
                sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position);
        void registerActions();

    private:
        static inline const std::string FONT_PATH = "resources/fonts/calibri.ttf";
        static inline const sf::Color& BACKGROUND_COLOR = sf::Color{70, 0, 0, 255};
        static inline const uint8_t TOTAL_TEXT_BUTTONS = 2;
        static inline const sf::Color& TITLE_TEXT_COLOUR = sf::Color::Red;
        static inline const sf::Color& BUTTON_DEFAULT_COLOR = sf::Color::White;
        static inline const sf::Color& BUTTON_HIGHLIGHT_COLOR = sf::Color{0, 150, 200};

        sf::Font m_font;
        uint8_t currentSelectItem = 0;
        std::pair<sf::Color, sf::Text> gameTitleTextPair;
        std::pair<sf::Color, sf::Text> startGameTextualButtonPair;
        std::pair<sf::Color, sf::Text> exitTextualButtonPair;
        sf::Cursor cursor;
        sf::Clock& m_deltaClock;
};

#endif //PRIMITIVE_WARS_MENU_SCENE_H
