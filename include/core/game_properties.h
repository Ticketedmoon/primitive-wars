#pragma once

#ifndef PRIMITIVE_WARS_GAME_PROPERTIES_H
#define PRIMITIVE_WARS_GAME_PROPERTIES_H

#include <cstddef>
#include <SFML/System/Clock.hpp>
#include <cstdint>

enum class Difficulty
{
    EASY,
    MEDIUM,
    HARD
};

class GameProperties
{
    public:
        explicit GameProperties(sf::Clock& deltaClock);
        explicit GameProperties(Difficulty difficulty, uint8_t totalLives, bool hasPaused, sf::Clock& worldClock,
                sf::Clock& deltaClock, float playerRespawnTimeSeconds, float specialAttackCoolDownSeconds,
                float enemySpawnRateSeconds, float enemySpeed);

        void resetPropertiesForLevel(sf::Clock& worldClock, float timeToCompletion);

        Difficulty getSelectedDifficulty();
        void setLevelDifficulty(Difficulty difficulty);
        [[nodiscard]] float getEnemySpawnRateSeconds() const;
        void setEnemySpawnRateSeconds(float enemySpawnRateSeconds);

        float getEnemySpeed();
        void setEnemySpeed(float enemySpeed);

        void setPaused(bool paused);
        [[nodiscard]] bool hasPaused() const;

        [[nodiscard]] uint8_t getTotalLives() const;
        void setTotalLives(uint8_t totalLives);

        [[nodiscard]] float getTimeRemainingBeforeVictory() const;

        sf::Clock& getDeltaClock();
        sf::Clock& getLevelClock();

        [[nodiscard]] float getPlayerRespawnTimeSeconds() const;
        [[nodiscard]] float getSpecialAttackCoolDownSeconds() const;
        void setPlayerRespawnTimeSeconds(float seconds);
        void setSpecialAttackCoolDownSeconds(float seconds);


    private:
        Difficulty m_difficulty;
        uint8_t m_totalLives{};
        bool m_paused{};

        sf::Clock m_levelClock;
        sf::Clock& m_deltaClock;

        float m_playerRespawnTimeSeconds{};
        float m_specialAttackCoolDownSeconds{};
        float m_timeRemainingBeforeVictory{};
        float m_enemySpawnRateSeconds{};
        float m_enemySpeed{};
};


#endif //PRIMITIVE_WARS_GAME_PROPERTIES_H
