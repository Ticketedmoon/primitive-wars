#include "scene/gameplay_scene/system/gui_system.h"

GuiSystem::GuiSystem(sf::RenderWindow& renderWindow, EntityManager& entityManager, sf::Clock& worldClock, GameProperties& gameProperties)
    : m_window(renderWindow), m_entityManager(entityManager), m_worldClock(worldClock), m_gameProperties(gameProperties)
{
    configureTextRendering();
}

void GuiSystem::execute()
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

    if (m_gameProperties.hasPaused)
    {
        drawText(pauseText, sf::Color::Green, 128, PAUSED_TEXT_SCREEN_POSITION);
        return;
    }

    if (isPlayerDead && isPlayerWaitingOnRespawnTime())
    {
        renderTextOnPlayerDeath();
    }
    else
    {
        renderCooldownText(players.front());
    }
}

void GuiSystem::renderTextOnPlayerDeath()
{
    uint8_t respawnTime =
            (m_gameProperties.playerRespawnTimeSeconds - m_worldClock.getElapsedTime().asSeconds()) + 1;
    respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
    drawText(respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 256, WINDOW_HEIGHT / 2 - 64));
}

void GuiSystem::renderCooldownText(std::shared_ptr<Entity>& player)
{
    std::shared_ptr<CScore> scoreComponent = std::static_pointer_cast<CScore> (player->getComponentByType(Component::Type::SCORE));

    uint8_t coolDownSeconds = m_worldClock.getElapsedTime().asSeconds() > m_gameProperties.specialAttackCoolDownSeconds
            ? 0.0f
            : std::ceil(m_gameProperties.specialAttackCoolDownSeconds - m_worldClock.getElapsedTime().asSeconds());
    const std::string text = "Score: " + std::to_string(scoreComponent->getScore()) + "\n"
            + "Deaths: " + std::to_string(m_gameProperties.totalDeaths) + "\n"
            + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);
    gameOverlayText.setString(text);
    drawText(gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));
}

bool GuiSystem::isPlayerWaitingOnRespawnTime() const
{
    return m_gameProperties.playerRespawnTimeSeconds > 0;
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
            m_gameProperties.playerRespawnTimeSeconds = (m_worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);
            m_gameProperties.totalDeaths++;
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

    gameOverlayText = sf::Text("", m_font);
    respawnText = sf::Text("", m_font);
    pauseText = sf::Text(PAUSED_TEXT, m_font);
}