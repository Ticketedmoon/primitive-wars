#pragma once

#ifndef PRIMITIVE_WARS_C_ACTION_H
#define PRIMITIVE_WARS_C_ACTION_H

#include "component.h"

class CAction : public Component
{
    public:
        bool isMovingLeft;
        bool isMovingRight;
        bool isMovingUp;
        bool isMovingDown;
        bool isShooting;
        bool isPerformingSpecialAttack;
        sf::Vector2f projectileDestination;
};


#endif //PRIMITIVE_WARS_C_ACTION_H
