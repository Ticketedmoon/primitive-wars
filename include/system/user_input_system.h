#pragma once

#ifndef PRIMITIVE_WARS_USER_INPUT_SYSTEM_H
#define PRIMITIVE_WARS_USER_INPUT_SYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "system.h"
#include "entity_manager.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <valarray>
#include "c_user_input.h"
#include "c_transform.h"
#include "gui_system.h"

class UserInputSystem : public System
{
    public:
        explicit UserInputSystem(EntityManager& entityManager, sf::RenderWindow& renderWindow, sf::Clock& worldClock,
                GuiProperties& guiProperties);

        void execute() override;

    private:
        EntityManager& m_entityManager;
        sf::RenderWindow& m_renderWindow;

        sf::Clock& m_worldClock;
        GuiProperties& m_guiProperties;
};


#endif //PRIMITIVE_WARS_USER_INPUT_SYSTEM_H
