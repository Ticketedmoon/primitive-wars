#pragma once

#ifndef PRIMITIVE_WARS_C_COLLISION_H
#define PRIMITIVE_WARS_C_COLLISION_H

#include "component.h"

class CCollision : public Component
{
    public:
        Type getType() override;

    public:
        bool isCollidingLeft = false;
        bool isCollidingRight = false;
        bool isCollidingUp = false;
        bool isCollidingDown = false;
};


#endif //PRIMITIVE_WARS_C_COLLISION_H
