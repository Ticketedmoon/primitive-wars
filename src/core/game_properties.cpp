#include "game_properties.h"

GameProperties::GameProperties()
{

}
GameProperties::GameProperties(Difficulty difficulty, uint8_t totalLives, bool hasPaused, sf::Clock& worldClock,
        float playerRespawnTimeSeconds, float specialAttackCoolDownSeconds, float enemySpawnRateSeconds,
        float enemySpeed) : m_difficulty(difficulty), m_totalLives(totalLives),
                            m_paused(hasPaused), m_levelClock(worldClock),
                            m_playerRespawnTimeSeconds(playerRespawnTimeSeconds),
                            m_specialAttackCoolDownSeconds(specialAttackCoolDownSeconds),
                            m_enemySpawnRateSeconds(enemySpawnRateSeconds),
                            m_enemySpeed(enemySpeed)
{

}

void GameProperties::resetPropertiesForLevel(sf::Clock& levelClock, float timeToCompletion)
{
    m_levelClock = levelClock;
    m_paused = false;
    m_totalLives = 3;
    m_playerRespawnTimeSeconds = 0;
    m_specialAttackCoolDownSeconds = 0;
    m_timeRemainingBeforeVictory = m_levelClock.getElapsedTime().asSeconds() + timeToCompletion;
}

float GameProperties::getEnemySpawnRateSeconds() const
{
    return m_enemySpawnRateSeconds;
}

Difficulty GameProperties::getSelectedDifficulty()
{
    return m_difficulty;
}

void GameProperties::setLevelDifficulty(Difficulty difficulty)
{
    m_difficulty = difficulty;
}

void GameProperties::setEnemySpawnRateSeconds(float enemySpawnRateSeconds)
{
    m_enemySpawnRateSeconds = enemySpawnRateSeconds;
}

float GameProperties::getEnemySpeed()
{
    return m_enemySpeed;
}

void GameProperties::setEnemySpeed(float enemySpeed)
{
    m_enemySpeed = enemySpeed;
}

bool GameProperties::hasPaused() const
{
    return m_paused;
}

void GameProperties::setPaused(bool paused)
{
    m_paused = paused;
}

float GameProperties::getTimeRemainingBeforeVictory() const
{
    return m_timeRemainingBeforeVictory;
}

uint8_t GameProperties::getTotalLives() const
{
    return m_totalLives;
}

void GameProperties::setTotalLives(uint8_t totalLives)
{
    m_totalLives = totalLives;
}

float GameProperties::getPlayerRespawnTimeSeconds() const
{
    return m_playerRespawnTimeSeconds;
}

float GameProperties::getSpecialAttackCoolDownSeconds() const
{
    return m_specialAttackCoolDownSeconds;
}

void GameProperties::setPlayerRespawnTimeSeconds(float seconds)
{
    m_playerRespawnTimeSeconds = seconds;
}

void GameProperties::setSpecialAttackCoolDownSeconds(float seconds)
{
    m_specialAttackCoolDownSeconds = seconds;
}

sf::Clock& GameProperties::getLevelClock()
{
    return m_levelClock;
}
