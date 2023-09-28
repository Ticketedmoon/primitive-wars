#pragma once

#ifndef PRIMITIVE_WARS_SYSTEM_H
#define PRIMITIVE_WARS_SYSTEM_H

#include "core/game_properties.h"

class System
{
    public:
        virtual ~System() = default;
        virtual void execute() = 0;
        virtual bool shouldApply(GameProperties gameProperties) = 0;
};

#endif //PRIMITIVE_WARS_SYSTEM_H
