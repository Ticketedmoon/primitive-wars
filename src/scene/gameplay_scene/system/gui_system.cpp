#include <iostream>
#include "scene/gameplay_scene/system/gui_system.h"

GuiSystem::GuiSystem(sf::RenderWindow& renderWindow, EntityManager& entityManager, GameProperties& gameProperties)
    : m_window(renderWindow), m_entityManager(entityManager), m_gameProperties(gameProperties)
{
    configureTextRendering();
}

void GuiSystem::execute(GameProperties& gameProperties)
{
    updateGuiData();
    drawGuiData();
}

bool GuiSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}

void GuiSystem::drawGuiData()
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    bool isPlayerDead = players.empty();

    if (m_gameProperties.hasPaused())
    {
        drawText(m_pauseText, sf::Color::Green, 128, PAUSED_TEXT_SCREEN_POSITION);
        return;
    }

    if (isPlayerDead && isPlayerWaitingOnRespawnTime())
    {
        renderTextOnPlayerDeath();
    }
    else
    {
        renderGameOverlayText(players.front());
    }
}

void GuiSystem::renderTextOnPlayerDeath()
{
    uint8_t respawnTime =
            (m_gameProperties.getPlayerRespawnTimeSeconds() - m_gameProperties.getLevelClock().getElapsedTime().asSeconds()) + 1;
    m_respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
    drawText(m_respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 256, WINDOW_HEIGHT / 2 - 64));
}

void GuiSystem::renderGameOverlayText(std::shared_ptr<Entity>& player)
{
    std::shared_ptr<CScore> scoreComponent = std::static_pointer_cast<CScore> (player->getComponentByType(Component::Type::SCORE));

    uint8_t coolDownSeconds = m_gameProperties.getLevelClock().getElapsedTime().asSeconds() > m_gameProperties.getSpecialAttackCoolDownSeconds()
            ? 0.0f
            : std::ceil(m_gameProperties.getSpecialAttackCoolDownSeconds() - m_gameProperties.getLevelClock().getElapsedTime().asSeconds());

    float timeRemaining = m_gameProperties.getTimeRemainingBeforeVictory() - m_gameProperties.getLevelClock().getElapsedTime().asSeconds();
    const std::string text = "Score: " + std::to_string(scoreComponent->getScore()) + "\n"
            + "Total Lives: " + std::to_string(m_gameProperties.getTotalLives()) + "\n"
            + "Time remaining: " + std::to_string(timeRemaining) + "\n"
            + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);
    m_gameOverlayText.setString(text);
    drawText(m_gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));
}

bool GuiSystem::isPlayerWaitingOnRespawnTime() const
{
    return m_gameProperties.getPlayerRespawnTimeSeconds() > 0;
}

void GuiSystem::updateGuiData()
{
    std::vector<std::shared_ptr<Entity>> destroyedEntities = m_entityManager.getDestroyedEntitiesByComponentTypes({Component::COLLISION});
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    if (players.empty())
    {
        return;
    }

    std::shared_ptr<CScore> playerScoreComponent = std::static_pointer_cast<CScore> (players.front()->getComponentByType(Component::Type::SCORE));
    for (const auto& e: destroyedEntities)
    {
        if (e->getType() == Entity::Type::PLAYER)
        {
            float levelTimeSeconds = m_gameProperties.getLevelClock().getElapsedTime().asSeconds();
            std::cout << "set: " << levelTimeSeconds << '\n';
            m_gameProperties.setPlayerRespawnTimeSeconds(
                    levelTimeSeconds + DEFAULT_RESPAWN_RATE_SECONDS);
            m_gameProperties.setTotalLives(m_gameProperties.getTotalLives()-1);
        }

        if (e->getType() == Entity::Type::ENEMY)
        {
            std::shared_ptr<CScore> enemyScoreComponent = std::static_pointer_cast<CScore>(
                    e->getComponentByType(Component::SCORE));
            playerScoreComponent->addToScore(enemyScoreComponent->getScore());
        }
    }
}

// TODO move to engine for inter-scene text drawing?
void GuiSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);
    text.setLetterSpacing(1.0f);

    m_window.draw(text);
}

void GuiSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    m_gameOverlayText = sf::Text("", m_font);
    m_respawnText = sf::Text("", m_font);
    m_pauseText = sf::Text(PAUSED_TEXT, m_font);
}
