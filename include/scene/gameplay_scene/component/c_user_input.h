#pragma once

#ifndef PRIMITIVE_WARS_C_USER_INPUT_H
#define PRIMITIVE_WARS_C_USER_INPUT_H

#include "component.h"

class CUserInput : public Component
{
    public:
        bool isMovingLeft;
        bool isMovingRight;
        bool isMovingUp;
        bool isMovingDown;
        bool mouseLeftClicked;
        bool mouseRightClicked;
        sf::Vector2f mouseClickPosition;
};


#endif //PRIMITIVE_WARS_C_USER_INPUT_H
