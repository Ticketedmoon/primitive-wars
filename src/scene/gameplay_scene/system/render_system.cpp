#include "scene/gameplay_scene/system/render_system.h"
#include "c_score.h"

RenderSystem::RenderSystem(sf::RenderWindow& window, EntityManager& entityManager)
    : m_renderWindow(window), m_entityManager(entityManager)
{
    configureTextRendering();
}

void RenderSystem::execute(GameProperties& gameProperties)
{
    drawEntities();
    drawGuiData(gameProperties);
}

bool RenderSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}

void RenderSystem::drawEntities()
{
    std::vector<std::shared_ptr<Entity>> entitiesToRender = m_entityManager
            .getEntitiesByComponentTypes({Component::RENDER, Component::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : entitiesToRender)
    {
        std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform> (e->getComponentByType(Component::TRANSFORM));
        std::shared_ptr<CRender> renderComponentForEntity = std::static_pointer_cast<CRender> (e->getComponentByType(Component::RENDER));

        sf::CircleShape& drawable = renderComponentForEntity->m_shape;
        drawable.setPosition(transformComponentForEntity->m_position);
        drawable.rotate(2);

        m_renderWindow.draw(drawable);
    }
}

void RenderSystem::drawGuiData(GameProperties& gameProperties)
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);
    bool isPlayerDead = players.empty();

    if (gameProperties.hasPaused())
    {
        drawText(m_pauseText, sf::Color::Green, 128, PAUSED_TEXT_SCREEN_POSITION);
        return;
    }

    if (isPlayerDead && isPlayerWaitingOnRespawnTime(gameProperties))
    {
        renderTextOnPlayerDeath(gameProperties);
    }
    else
    {
        renderGameOverlayText(players.front(), gameProperties);
    }
}

void RenderSystem::renderTextOnPlayerDeath(GameProperties& gameProperties)
{
    uint8_t respawnTime =
            (gameProperties.getPlayerRespawnTimeSeconds() - gameProperties.getLevelClock().getElapsedTime().asSeconds()) + 1;
    m_respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
    drawText(m_respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 256, WINDOW_HEIGHT / 2 - 64));
}

void RenderSystem::renderGameOverlayText(std::shared_ptr<Entity>& player, GameProperties& gameProperties)
{
    std::shared_ptr<CScore> scoreComponent = std::static_pointer_cast<CScore> (player->getComponentByType(Component::Type::SCORE));

    uint8_t coolDownSeconds = gameProperties.getLevelClock().getElapsedTime().asSeconds() > gameProperties.getSpecialAttackCoolDownSeconds()
            ? 0.0f
            : std::ceil(gameProperties.getSpecialAttackCoolDownSeconds() - gameProperties.getLevelClock().getElapsedTime().asSeconds());

    float timeRemaining = gameProperties.getTimeRemainingBeforeVictory() - gameProperties.getLevelClock().getElapsedTime().asSeconds();
    const std::string text = "Score: " + std::to_string(scoreComponent->getScore()) + "\n"
            + "Total Lives: " + std::to_string(gameProperties.getTotalLives()) + "\n"
            + "Time remaining: " + std::to_string(timeRemaining) + "\n"
            + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);
    m_gameOverlayText.setString(text);
    drawText(m_gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));
}

bool RenderSystem::isPlayerWaitingOnRespawnTime(GameProperties& gameProperties) const
{
    return gameProperties.getPlayerRespawnTimeSeconds() > 0;
}

// TODO move to engine for inter-scene text drawing?
void RenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);
    text.setLetterSpacing(1.0f);

    m_renderWindow.draw(text);
}

void RenderSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    m_gameOverlayText = sf::Text("", m_font);
    m_respawnText = sf::Text("", m_font);
    m_pauseText = sf::Text(PAUSED_TEXT, m_font);
}
