#include "gui_system.h"

GuiSystem::GuiSystem(sf::RenderWindow& renderWindow, EntityManager& entityManager, sf::Clock& worldClock, GuiProperties& guiProperties)
    : m_window(renderWindow), m_entityManager(entityManager), m_worldClock(worldClock), m_guiProperties(guiProperties)
{
    configureTextRendering();
}

void GuiSystem::execute()
{
    updateGuiData();
    drawGuiData();
}

void GuiSystem::drawGuiData()
{
    std::vector<std::shared_ptr<Entity>>& players = m_entityManager.getEntitiesByType(Entity::Type::PLAYER);

    if (m_guiProperties.hasPaused)
    {
        drawText(pauseText, sf::Color::Green, 128, PAUSED_TEXT_SCREEN_POSITION);
    }
    else if (players.empty())
    {
        uint8_t respawnTime = (m_guiProperties.playerRespawnTimeSeconds - m_worldClock.getElapsedTime().asSeconds()) + 1;
        respawnText.setString("Respawn Time: " + std::to_string(respawnTime));
        drawText(respawnText, sf::Color::Yellow, 72, sf::Vector2f(WINDOW_WIDTH / 2 - 256, WINDOW_HEIGHT / 2 - 64));
    }

    uint8_t coolDownSeconds = m_worldClock.getElapsedTime().asSeconds() > m_guiProperties.specialAttackCoolDownSeconds
            ? 0.0f
            : std::ceil(m_guiProperties.specialAttackCoolDownSeconds - m_worldClock.getElapsedTime().asSeconds());
    const std::string text = "Score: " + std::to_string(m_guiProperties.totalScore) + "\n"
            + "Deaths: " + std::to_string(m_guiProperties.totalDeaths) + "\n"
            + "Special Attack Cooldown: " + std::to_string(coolDownSeconds);
    gameOverlayText.setString(text);

    drawText(gameOverlayText, sf::Color::White, 20, sf::Vector2f(24, 12));
}

void GuiSystem::updateGuiData()
{
    std::vector<std::shared_ptr<Entity>> destroyedEntities = m_entityManager
            .getDestroyedEntitiesByComponentTypes({Component::COLLISION});
    for (const auto& e: destroyedEntities)
    {
        if (e->getType() == Entity::Type::PLAYER)
        {
            m_guiProperties.playerRespawnTimeSeconds = (m_worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);
            m_guiProperties.totalDeaths++;
        }

        if (e->getType() == Entity::Type::ENEMY)
        {
            std::shared_ptr<CRender> renderComponent = std::static_pointer_cast<CRender>(
                    e->getComponentByType(Component::RENDER));
            m_guiProperties.totalScore += (100 * renderComponent->m_shape.getPointCount());
        }
    }
}

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
