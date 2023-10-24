#pragma once

#ifndef PRIMITIVE_WARS_GUI_UPDATER_SYSTEM_H
#define PRIMITIVE_WARS_GUI_UPDATER_SYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>

#include "c_score.h"
#include "c_render.h"
#include "system.h"
#include "entity/entity_manager.h"
#include "core/common_constants.h"
#include "core/game_properties.h"

class GuiUpdaterSystem : public System
{
    public:
        explicit GuiUpdaterSystem(EntityManager& entityManager, GameProperties& gameProperties);

        void execute(GameProperties& gameProperties) override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        EntityManager& m_entityManager;
        GameProperties& m_gameProperties;
};


#endif //PRIMITIVE_WARS_GUI_UPDATER_SYSTEM_H
