#pragma once

#ifndef PRIMITIVE_WARS_C_USER_INPUT_H
#define PRIMITIVE_WARS_C_USER_INPUT_H

#include "component.h"

class CUserInput : public Component
{
    public:
        Type getType() override;

        bool movingLeft;
        bool movingRight;
        bool movingUp;
        bool movingDown;
        bool isShooting;
};


#endif //PRIMITIVE_WARS_C_USER_INPUT_H
