#pragma once

#ifndef PRIMITIVE_WARS_C_USER_INPUT_H
#define PRIMITIVE_WARS_C_USER_INPUT_H

#include "component.h"

class CUserInput : public Component
{
    public:
        Type getType() override;

        bool isMovingLeft;
        bool isMovingRight;
        bool isMovingUp;
        bool isMovingDown;
        bool isMousePressed;
};


#endif //PRIMITIVE_WARS_C_USER_INPUT_H
