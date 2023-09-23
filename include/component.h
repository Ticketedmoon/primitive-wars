#pragma once

#ifndef PRIMITIVE_WARS_COMPONENT_H
#define PRIMITIVE_WARS_COMPONENT_H

#include <SFML/System/Vector2.hpp>

class Component
{
    public:
        enum class Type
        {
                TRANSFORM,
                USER_INPUT,
                RENDER,
                COLLISION,
                LIFESPAN
        };

        virtual Type getType() = 0;

    private:
        Type m_type;
};


#endif //PRIMITIVE_WARS_COMPONENT_H
