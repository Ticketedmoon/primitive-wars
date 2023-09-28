#pragma once

#ifndef PRIMITIVE_WARS_MENU_SCENE_H
#define PRIMITIVE_WARS_MENU_SCENE_H

#include <SFML/Graphics/RectangleShape.hpp>

#include "scene/menu_scene/system/menu_user_input_system.h"
#include "game_engine.h"
#include "scene.h"

class MenuScene : public Scene
{
    public:
        explicit MenuScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        static const inline std::string FONT_PATH = "resources/fonts/calibri.ttf";

        static inline SystemManager m_systemManager;
        static inline EntityManager m_entityManager;

        sf::Font m_font;
        sf::Text gameTitleText;
        sf::Text startGameTextualButton;
        sf::Text exitTextualButton;

        void createTextElement(sf::Text& text, const std::string& value, uint16_t characterSize, sf::Color fillColor,
                sf::Color outlineColor, float outlineThickness, sf::Vector2f position);
};

#endif //PRIMITIVE_WARS_MENU_SCENE_H
