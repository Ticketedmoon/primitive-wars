#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void TransformSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::TRANSFORM});
    for (const std::shared_ptr<Entity>& e: entities)
    {
        std::shared_ptr<CTransform> transformComponent = std::static_pointer_cast<CTransform>(e->getComponentByType(
                Component::Type::TRANSFORM));
        if (e->hasComponents({Component::Type::COLLISION}))
        {
            std::shared_ptr<CCollision> collisionComponent = std::static_pointer_cast<CCollision>(e->getComponentByType(
                    Component::Type::COLLISION));
            if (e->hasComponent(Component::USER_INPUT))
            {
                updateEntityTransformByUserInput(e, transformComponent, collisionComponent);
            }
            else
            {
                updateTransformOnWindowBorderCollision(transformComponent, collisionComponent);
            }
        }

        transformComponent->m_position.x += (transformComponent->m_speed.x * transformComponent->m_speedDelta.x);
        transformComponent->m_position.y += (transformComponent->m_speed.y * transformComponent->m_speedDelta.y);
    }
}

void TransformSystem::updateEntityTransformByUserInput(const std::shared_ptr<Entity>& e,
        std::shared_ptr<CTransform>& transformComponent, const std::shared_ptr<CCollision>& collisionComponent)
{
    std::shared_ptr<CUserInput> userInputComponent = std::static_pointer_cast<CUserInput>(e->getComponentByType(
            Component::USER_INPUT));

    if (!collisionComponent->isCollidingLeft)
    {
        transformComponent->m_position.x -= userInputComponent->isMovingLeft ? transformComponent->m_speed.x : 0;
    }
    if (!collisionComponent->isCollidingRight)
    {
        transformComponent->m_position.x += userInputComponent->isMovingRight ? transformComponent->m_speed.x : 0;
    }
    if (!collisionComponent->isCollidingUp)
    {
        transformComponent->m_position.y -= userInputComponent->isMovingUp ? transformComponent->m_speed.y : 0;
    }
    if (!collisionComponent->isCollidingDown)
    {
        transformComponent->m_position.y += userInputComponent->isMovingDown ? transformComponent->m_speed.y : 0;
    }
}

void TransformSystem::updateTransformOnWindowBorderCollision(std::shared_ptr<CTransform>& transformComponent,
        const std::shared_ptr<CCollision>& collisionComponent)
{
    if (collisionComponent->isCollidingLeft || collisionComponent->isCollidingRight)
    {
        transformComponent->m_speed.x = -transformComponent->m_speed.x;
    }
    if (collisionComponent->isCollidingUp || collisionComponent->isCollidingDown)
    {
        transformComponent->m_speed.y = -transformComponent->m_speed.y;
    }
}
