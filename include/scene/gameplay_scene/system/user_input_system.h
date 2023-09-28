#pragma once

#ifndef PRIMITIVE_WARS_USER_INPUT_SYSTEM_H
#define PRIMITIVE_WARS_USER_INPUT_SYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "system.h"
#include "entity/entity_manager.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <valarray>
#include "c_user_input.h"
#include "c_transform.h"
#include "gui_system.h"

class UserInputSystem : public System
{
    public:
        explicit UserInputSystem(sf::RenderWindow& renderWindow, EntityManager& entityManager, sf::Clock& worldClock,
                GameProperties& gameProperties);

        void execute() override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        sf::RenderWindow& m_renderWindow;
        EntityManager& m_entityManager;

        sf::Clock& m_worldClock;
        GameProperties& m_gameProperties;
};


#endif //PRIMITIVE_WARS_USER_INPUT_SYSTEM_H
