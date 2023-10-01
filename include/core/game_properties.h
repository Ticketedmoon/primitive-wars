#pragma once

#ifndef PRIMITIVE_WARS_GAME_PROPERTIES_H
#define PRIMITIVE_WARS_GAME_PROPERTIES_H

#include <cstddef>
#include <SFML/System/Clock.hpp>

enum class Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

struct GameplayProperties
{
    Difficulty difficulty;
    float enemySpawnRateSeconds;
    float enemySpeed;
};

class GameProperties
{
    public:
        Difficulty difficulty;
        bool hasPaused;
        uint8_t totalLives;
        sf::Clock worldClock;
        float playerRespawnTimeSeconds;
        float specialAttackCoolDownSeconds;
        float timeRemainingBeforeVictory;
        float enemySpawnRateSeconds;
        float enemySpeed;
};

#endif //PRIMITIVE_WARS_GAME_PROPERTIES_H
