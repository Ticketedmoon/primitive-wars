#pragma once

#ifndef PRIMITIVE_WARS_GAME_PROPERTIES_H
#define PRIMITIVE_WARS_GAME_PROPERTIES_H

#include <cstddef>

class GameProperties
{
    public:
        bool hasPaused;
        size_t totalScore;
        size_t totalDeaths;
        float playerRespawnTimeSeconds;
        float specialAttackCoolDownSeconds;
};

#endif //PRIMITIVE_WARS_GAME_PROPERTIES_H
