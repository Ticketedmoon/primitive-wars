#include "render_system.h"

RenderSystem::RenderSystem(sf::RenderWindow& window, EntityManager& entityManager, sf::Clock& worldClock,
        float& specialAttackCoolDownSeconds, size_t& score, size_t& totalDeaths,
        float& playerRespawnTimeSeconds, bool& hasPaused)
    : m_renderWindow(window), m_entityManager(entityManager), m_worldClock(worldClock),
    m_specialAttackCoolDownSeconds(specialAttackCoolDownSeconds), m_score(score), m_totalDeaths(totalDeaths),
    m_playerRespawnTimeSeconds(playerRespawnTimeSeconds), hasPaused(hasPaused)
{
    configureTextRendering();

    bool isFileLoaded = textureSprite.loadFromFile(BACKGROUND_IMAGE_PATH);
    assert(isFileLoaded);

    backgroundSprite = sf::Sprite(textureSprite);
}

void RenderSystem::execute()
{
    m_renderWindow.draw(backgroundSprite);

    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::RENDER, Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform> (e->getComponentByType(Component::Type::TRANSFORM));
        std::shared_ptr<CRender> renderComponentForEntity = std::static_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));

        sf::CircleShape& drawable = renderComponentForEntity->m_shape;
        drawable.setPosition(transformComponentForEntity->m_position);
        drawable.rotate(2);

        m_renderWindow.draw(drawable);
    }

    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);

    if (hasPaused)
    {
        drawText(pauseText, sf::Color::Green, 128, CENTRE_SCREEN_POSITION);
        drawText(gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));
        return;
    }
    else if (players.empty())
    {
        uint8_t respawnTime = (m_playerRespawnTimeSeconds - m_worldClock.getElapsedTime().asSeconds()) + 1;
        respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
        drawText(respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 324, WINDOW_HEIGHT / 2 - 64));
    }
    else
    {
        uint8_t coolDownSeconds = m_worldClock.getElapsedTime().asSeconds() > m_specialAttackCoolDownSeconds
                ? 0.0f
                : std::ceil(m_specialAttackCoolDownSeconds - m_worldClock.getElapsedTime().asSeconds());
        const std::string text = "Score: " + std::to_string(m_score) + "\n"
                + "Deaths: " + std::to_string(m_totalDeaths) + "\n"
                + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);
        gameOverlayText.setString(text);

        drawText(gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));
    }
}

void RenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    // TODO parameterise us
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(3.0f);
    text.setLetterSpacing(3.0f);

    m_renderWindow.draw(text);
}

void RenderSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    gameOverlayText = sf::Text("", m_font);
    respawnText = sf::Text("", m_font);
    pauseText = sf::Text(PAUSED_TEXT, m_font);
}
