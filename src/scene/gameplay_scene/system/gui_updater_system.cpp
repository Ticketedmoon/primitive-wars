#include "scene/gameplay_scene/system/gui_updater_system.h"

GuiUpdaterSystem::GuiUpdaterSystem(EntityManager& entityManager, GameProperties& gameProperties)
    : m_entityManager(entityManager), m_gameProperties(gameProperties)
{
}

void GuiUpdaterSystem::execute(GameProperties& gameProperties)
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

bool GuiUpdaterSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}