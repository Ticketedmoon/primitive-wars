#include "collision_system.h"
#include "common_constants.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager, sf::Clock& worldClock, size_t& score,
        float& playerRespawnSeconds, size_t& totalDeaths)
        : m_entityManager(entityManager), m_worldClock(worldClock), m_score(score),
          m_playerRespawnSeconds(playerRespawnSeconds), m_totalDeaths(totalDeaths)
{

}

void CollisionSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> bulletsToUpdate = m_entityManager.getEntitiesByType(Entity::Type::BULLET);
    for (const std::shared_ptr<Entity>& bullet: bulletsToUpdate)
    {
        std::shared_ptr<CRender> bulletRenderComponent = std::static_pointer_cast<CRender>(bullet->getComponentByType(Component::Type::RENDER));
        std::ranges::filter_view enemiesFiltered = m_entityManager.getEntitiesByType(Entity::Type::ENEMY) |
                std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
                    return e->hasComponent(Component::Type::TRANSFORM) &&
                            e->hasComponent(Component::Type::COLLISION) &&
                            e->hasComponent(Component::Type::RENDER);
                });
        std::vector<std::shared_ptr<Entity>> enemiesToUpdate = std::vector(enemiesFiltered.begin(), enemiesFiltered.end());
        for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
        {
            std::shared_ptr<CTransform> transformComponentForEnemy = std::static_pointer_cast<CTransform>(enemy->getComponentByType(Component::Type::TRANSFORM));
            std::shared_ptr<CRender> enemyRenderComponent = std::static_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            if (isCollidingAABB(bulletRenderComponent, enemyRenderComponent))
            {
                // destroy enemy
                enemy->destroy();

                // destroy bullet
                bullet->destroy();

                // update score
                size_t totalVertices = enemyRenderComponent->m_shape.getPointCount();
                m_score += (100 * totalVertices);
            }
        }
    }

    std::ranges::filter_view playersFiltered = m_entityManager.getEntitiesByType(Entity::Type::PLAYER) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::RENDER);
    });
    std::ranges::filter_view enemiesFiltered = m_entityManager.getEntitiesByType(Entity::Type::ENEMY) | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
        return e->hasComponent(Component::Type::TRANSFORM) && e->hasComponent(Component::Type::COLLISION) && e->hasComponent(Component::Type::RENDER);
    });

    std::vector<std::shared_ptr<Entity>> enemiesToUpdate = std::vector(enemiesFiltered.begin(), enemiesFiltered.end());
    std::vector<std::shared_ptr<Entity>> playersToUpdate = std::vector(playersFiltered.begin(), playersFiltered.end());

    for (const std::shared_ptr<Entity>& player: playersToUpdate)
    {
        checkForWindowCollision(player);
        for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
        {
            std::shared_ptr<CRender> playerRenderComponent = std::static_pointer_cast<CRender>(player->getComponentByType(Component::Type::RENDER));
            std::shared_ptr<CRender> enemyRenderComponent = std::static_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            if (isCollidingAABB(playerRenderComponent, enemyRenderComponent))
            {
                // destroy player
                player->destroy();

                m_playerRespawnSeconds = (m_worldClock.getElapsedTime().asSeconds() + DEFAULT_RESPAWN_RATE_SECONDS);

                // destroy enemy
                enemy->destroy();

                m_totalDeaths++;
            }
        }
    }

    for (const std::shared_ptr<Entity>& enemy: enemiesToUpdate)
    {
        checkForWindowCollision(enemy);
    }
}

void CollisionSystem::checkForWindowCollision(const std::shared_ptr<Entity>& e) const
{
    std::shared_ptr<CTransform> transformComponentForEntity = std::static_pointer_cast<CTransform>(e->getComponentByType(Component::Type::TRANSFORM));
    std::shared_ptr<CCollision> collisionComponentForEntity = std::static_pointer_cast<CCollision>(e->getComponentByType(Component::Type::COLLISION));
    if (collisionComponentForEntity != nullptr)
    {
        std::shared_ptr<CRender> renderComponentForEntity = std::static_pointer_cast<CRender>(e->getComponentByType(Component::Type::RENDER));

        collisionComponentForEntity->isCollidingLeft = transformComponentForEntity->m_position.x <=
                renderComponentForEntity->m_shape.getRadius();

        collisionComponentForEntity->isCollidingRight = transformComponentForEntity->m_position.x >=
                WINDOW_WIDTH - renderComponentForEntity->m_shape.getRadius();

        collisionComponentForEntity->isCollidingUp = transformComponentForEntity->m_position.y <=
                renderComponentForEntity->m_shape.getRadius();

        collisionComponentForEntity->isCollidingDown = transformComponentForEntity->m_position.y >=
                WINDOW_HEIGHT - renderComponentForEntity->m_shape.getRadius();
    }
}

bool CollisionSystem::isCollidingAABB(
        const std::shared_ptr<CRender>& renderComponentForEntity,
        const std::shared_ptr<CRender>& renderComponentForEnemy)
{
    return renderComponentForEntity->m_shape.getGlobalBounds()
            .intersects(renderComponentForEnemy->m_shape.getGlobalBounds());
}
