#pragma once

#ifndef PRIMITIVE_WARS_GAME_PROPERTIES_H
#define PRIMITIVE_WARS_GAME_PROPERTIES_H

#include <cstddef>
#include <SFML/System/Clock.hpp>

class GameProperties
{
    public:
        bool hasPaused;
        size_t totalScore;
        size_t totalDeaths;
        sf::Clock worldClock;
        float playerRespawnTimeSeconds;
        float specialAttackCoolDownSeconds;
};

#endif //PRIMITIVE_WARS_GAME_PROPERTIES_H
