#pragma once

#ifndef PRIMITIVE_WARS_COMPONENT_H
#define PRIMITIVE_WARS_COMPONENT_H

#include <SFML/System/Vector2.hpp>

class Component
{
    public:
        enum Type
        {
            TRANSFORM = 0,
            USER_INPUT = 1,
            COLLISION = 2,
            LIFESPAN = 3,
            RENDER = 4,
            SCORE = 5,
            COUNT = 6
        };
};


#endif //PRIMITIVE_WARS_COMPONENT_H
