#pragma once

#ifndef PRIMITIVE_WARS_RENDER_SYSTEM_H
#define PRIMITIVE_WARS_RENDER_SYSTEM_H

#include <cmath>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "system.h"
#include "entity_manager.h"
#include "c_render.h"
#include "c_transform.h"
#include "common_constants.h"

class RenderSystem : public System
{
    public:
        explicit RenderSystem(sf::RenderWindow& window, EntityManager& entityManager);

        void execute() override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        sf::RenderWindow& m_renderWindow;
        EntityManager& m_entityManager;;
};


#endif //PRIMITIVE_WARS_RENDER_SYSTEM_H
