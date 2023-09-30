#pragma once

#ifndef PRIMITIVE_WARS_GAME_OVER_SCENE_H
#define PRIMITIVE_WARS_GAME_OVER_SCENE_H

#include <SFML/Graphics/RectangleShape.hpp>

#include "game_engine.h"
#include "scene.h"

class GameOverScene : public Scene
{
    public:
        explicit GameOverScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void handleMouseHover();
        void handleMouseClick();
        void onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover);
        void changeToMenuScene();

        std::pair<sf::Color, sf::Text> createTextElementPair(const std::string& value, uint16_t characterSize,
                sf::Color fillColor, sf::Color outlineColor, float outlineThickness, sf::Vector2f position);

    private:
        static inline const std::string FONT_PATH = "resources/fonts/calibri.ttf";
        static inline const sf::Color& BACKGROUND_COLOR = sf::Color::Black;
        static inline const sf::Color& TITLE_TEXT_COLOUR = sf::Color::Red;
        static inline const sf::Color& BUTTON_DEFAULT_COLOR = sf::Color::White;
        static inline const sf::Color& BUTTON_HIGHLIGHT_COLOR = sf::Color{0, 150, 200};

        sf::Font m_font;
        std::pair<sf::Color, sf::Text> gameOverText;
        std::pair<sf::Color, sf::Text> gameStatsText;
        std::pair<sf::Color, sf::Text> menuReturnTextButton;
        sf::Cursor cursor;
};

#endif //PRIMITIVE_WARS_GAME_OVER_SCENE_H
