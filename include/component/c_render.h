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
        explicit CRender(sf::CircleShape& shape);

    public:
        sf::CircleShape m_shape;
};


#endif //PRIMITIVE_WARS_C_RENDER_H
