#pragma once

#ifndef PRIMITIVE_WARS_C_RENDER_H
#define PRIMITIVE_WARS_C_RENDER_H

#include <cstdint>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "component.h"

class CRender : public Component
{
    public:
        Type getType() override;

    public:
        sf::CircleShape renderBody;
        sf::Color color;
};


#endif //PRIMITIVE_WARS_C_RENDER_H
