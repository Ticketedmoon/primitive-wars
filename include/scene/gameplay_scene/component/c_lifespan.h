#pragma once

#ifndef PRIMITIVE_WARS_C_LIFESPAN_H
#define PRIMITIVE_WARS_C_LIFESPAN_H

#include <cstdint>
#include "component.h"

class CLifespan : public Component
{
    public:
        explicit CLifespan(uint8_t timeToLive);

        [[nodiscard]] bool isAlive() const;
        void decreaseTimeToLive();

    private:
        uint8_t m_timeToLive;
};


#endif //PRIMITIVE_WARS_C_LIFESPAN_H
