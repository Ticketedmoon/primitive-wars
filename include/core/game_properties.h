#pragma once

#ifndef PRIMITIVE_WARS_GAME_PROPERTIES_H
#define PRIMITIVE_WARS_GAME_PROPERTIES_H

#include <cstddef>
#include <SFML/System/Clock.hpp>

class GameProperties
{
    public:
        bool hasPaused;
        uint8_t totalLives;
        sf::Clock worldClock;
        float playerRespawnTimeSeconds;
        float specialAttackCoolDownSeconds;
        float timeRemainingBeforeVictory;
};

#endif //PRIMITIVE_WARS_GAME_PROPERTIES_H
