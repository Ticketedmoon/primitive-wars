#include "entity_spawn_system.h"

EntitySpawnSystem::EntitySpawnSystem(EntityManager& entityManager)
        : m_entityManager(entityManager)
{
    spawnPlayer();
}

void EntitySpawnSystem::execute()
{

    std::vector<std::shared_ptr<Entity>>& enemies = m_entityManager.getEntitiesByType(Entity::Type::ENEMY);
    for (const auto& enemy: enemies)
    {
        if (!enemy->isAlive())
        {
            std::shared_ptr<CRender> enemyRenderComponent = std::static_pointer_cast<CRender>(enemy->getComponentByType(Component::Type::RENDER));
            size_t totalVertices = enemyRenderComponent->m_shape.getPointCount();
            spawnEntityAnimation(enemy, SpawnProperties(totalVertices, Entity::Type::ENEMY, false, sf::Vector2f(2.0f, 2.0f)));
        }
    }

}

void EntitySpawnSystem::spawnPlayer()
{
    std::shared_ptr<Entity>& player = m_entityManager.addEntity(Entity::Type::PLAYER);

    const sf::Vector2f& position = sf::Vector2f(10.0f, 10.0f);
    sf::CircleShape shape = createShape({20.0f, 3, position, sf::Color::Black, sf::Color::Red, 5.0f});

    player->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), position);
    player->m_components[Component::Type::COLLISION] = std::make_shared<CCollision>();
    player->m_components[Component::Type::USER_INPUT] = std::make_shared<CUserInput>();
    player->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void EntitySpawnSystem::spawnEnemy()
{
    std::shared_ptr<Entity>& enemy = m_entityManager.addEntity(Entity::Type::ENEMY);

    const int radius = std::experimental::randint(10, 50);
    const int x = std::experimental::randint(radius, static_cast<int>(WINDOW_WIDTH - radius));
    const int y = std::experimental::randint(radius, static_cast<int>(WINDOW_HEIGHT - radius));

    const sf::Vector2f position = isNearPlayer(sf::FloatRect(x, y, radius, radius))
            ? sf::Vector2f(radius * 2, radius * 2)
            : sf::Vector2f(x, y);
    const uint8_t totalVertices = std::experimental::randint(3, 10);
    const sf::Color fillColor = sf::Color(std::experimental::randint(50, 255), std::experimental::randint(50, 255),
            std::experimental::randint(50, 255), 255);

    sf::CircleShape shape = createShape({static_cast<float>(radius), totalVertices, position, fillColor, sf::Color::White, 3.0f});

    enemy->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(shape.getPosition(),
            sf::Vector2f(std::experimental::randint(1, 3), std::experimental::randint(1, 3)), sf::Vector2f(1, 1));
    enemy->m_components[Component::Type::COLLISION] = std::make_shared<CCollision>();
    enemy->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void EntitySpawnSystem::spawnEntityClone(const std::shared_ptr<Entity>& existingEntity, const SpawnProperties& spawnProperties,
        double shotAngle)
{
    std::shared_ptr<Entity>& enemyDeathAnimationEntity = m_entityManager.addEntity(spawnProperties.entityType);

    sf::CircleShape shapeForExistingEnemy =
            std::static_pointer_cast<CRender>(existingEntity->getComponentByType(Component::Type::RENDER))->m_shape;

    float radius = shapeForExistingEnemy.getRadius() / 3;
    sf::CircleShape shape = createShape({radius, shapeForExistingEnemy.getPointCount(), shapeForExistingEnemy.getPosition(),
                                         shapeForExistingEnemy.getFillColor(), shapeForExistingEnemy.getOutlineColor(),
                                         shapeForExistingEnemy.getOutlineThickness()});

    sf::Vector2f enemyAnimationPosition =
            std::static_pointer_cast<CTransform>(existingEntity->getComponentByType(Component::Type::TRANSFORM))->m_position;

    enemyDeathAnimationEntity->m_components[Component::Type::COLLISION] = spawnProperties.isCollidable ? std::make_shared<CCollision>() : nullptr;
    enemyDeathAnimationEntity->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(enemyAnimationPosition,
            spawnProperties.speed, sf::Vector2f(cos(shotAngle) * 1.0f, sin(shotAngle) * 1.0f));
    enemyDeathAnimationEntity->m_components[Component::Type::LIFESPAN] = std::make_shared<CLifespan>(100);
    enemyDeathAnimationEntity->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void EntitySpawnSystem::spawnBullet(sf::Vector2f position, double shotAngle)
{
    std::shared_ptr<Entity>& e = m_entityManager.addEntity(Entity::Type::BULLET);

    sf::CircleShape shape = createShape({10, 32, position, sf::Color::White, sf::Color::Black, 1.0f});

    e->m_components[Component::Type::TRANSFORM] = std::make_shared<CTransform>(position, sf::Vector2f(10.0f, 10.0f),
            sf::Vector2f(cos(shotAngle) * 1.0f, sin(shotAngle) * 1.0f));
    e->m_components[Component::Type::COLLISION] = std::make_shared<CCollision>();
    e->m_components[Component::Type::LIFESPAN] = std::make_shared<CLifespan>(100);
    e->m_components[Component::Type::RENDER] = std::make_shared<CRender>(shape);
}

void EntitySpawnSystem::spawnEntityAnimation(const std::shared_ptr<Entity>& existingEntity, const SpawnProperties& spawnProperties)
{
    for (size_t i = 0; i < spawnProperties.totalVertices; i++)
    {
        double shotAngle = (PI_FULL_CIRCLE / spawnProperties.totalVertices) * i;
        spawnEntityClone(existingEntity, spawnProperties, shotAngle);
    }
}

sf::CircleShape EntitySpawnSystem::createShape(ShapeProperties properties)
{
    float radius = properties.radius;
    sf::CircleShape shape(radius, properties.totalVertices);
    shape.setOrigin(radius, radius);
    shape.setPosition(properties.position);
    shape.setFillColor(properties.fillColor);
    shape.setOutlineColor(properties.outlineColor);
    shape.setOutlineThickness(properties.outlineThickness);
    return shape;
}

/*
 * This is about nearness and not collision.
 * We do not want an enemy to spawn on-top or even close-by to the player as this is either impossible or very difficult
 * to react to, and does not provide a good user experience.
 */
bool EntitySpawnSystem::isNearPlayer(sf::FloatRect enemyBoundingBox)
{
    const std::shared_ptr<CRender>& renderComponentForPlayer = std::static_pointer_cast<CRender>(
            m_entityManager.getEntityByType(Entity::Type::PLAYER)->getComponentByType(Component::Type::RENDER));

    // Make player rect larger for this calculation so enemies are not 'near'
    int offsetPx = 256;
    sf::FloatRect playerBoundingBox = renderComponentForPlayer->m_shape.getGlobalBounds();
    playerBoundingBox.left = (playerBoundingBox.left - offsetPx) < 0 ? 0 : (playerBoundingBox.left - offsetPx);
    playerBoundingBox.top = (playerBoundingBox.top - offsetPx) < 0 ? 0 : (playerBoundingBox.top - offsetPx);
    playerBoundingBox.width = playerBoundingBox.width + offsetPx;
    playerBoundingBox.height = playerBoundingBox.height + offsetPx;
    return playerBoundingBox.contains(sf::Vector2f(enemyBoundingBox.left, enemyBoundingBox.top));
}
