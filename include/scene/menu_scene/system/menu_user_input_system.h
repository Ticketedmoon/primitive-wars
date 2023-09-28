#pragma once

#ifndef PRIMITIVE_WARS_USER_INPUT_SYSTEM_H
#define PRIMITIVE_WARS_USER_INPUT_SYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <valarray>
#include <SFML/Graphics/RectangleShape.hpp>

#include "system.h"
#include "entity/entity_manager.h"
#include "c_user_input.h"
#include "c_transform.h"
#include "scene/gameplay_scene/system/gui_system.h"
#include "game_engine.h"

class MenuUserInputSystem : public System
{
    public:
        explicit MenuUserInputSystem(GameEngine& gameEngine, EntityManager& entityManager,
                sf::Text& startGameTextualButton, sf::Text& exitTextualButton);

        void execute() override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        void handleMouseClick();
        void onHover(sf::Text& text, sf::Color color, sf::Cursor::Type cursorTypeOnHover);

    private:
        GameEngine& gameEngine;
        EntityManager& m_entityManager;
        std::pair<sf::Color, sf::Text&> startGameTextualButtonPair;
        std::pair<sf::Color, sf::Text&> exitTextualButtonPair;
        sf::Cursor cursor;
        void handleMouseHover();
};


#endif //PRIMITIVE_WARS_USER_INPUT_SYSTEM_H
