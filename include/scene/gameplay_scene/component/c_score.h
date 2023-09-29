#pragma once

#ifndef PRIMITIVE_WARS_C_SCORE_H
#define PRIMITIVE_WARS_C_SCORE_H

#include <cstddef>
#include "component.h"

class CScore : public Component
{
    public:
        CScore() = default;
        CScore(size_t score);

        size_t getScore();
        void addToScore(size_t score);

    private:
        size_t m_score = 0;
};


#endif //PRIMITIVE_WARS_C_SCORE_H
