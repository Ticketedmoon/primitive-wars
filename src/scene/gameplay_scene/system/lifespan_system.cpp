#include "scene/gameplay_scene/system/lifespan_system.h"

LifespanSystem::LifespanSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LifespanSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entitiesToUpdate = m_entityManager
            .getEntitiesByComponentTypes({Component::Type::LIFESPAN, Component::Type::RENDER});

    for (const std::shared_ptr<Entity>& e : entitiesToUpdate)
    {
        std::shared_ptr<CLifespan> lifeSpanComponent = std::static_pointer_cast<CLifespan> (e->getComponentByType(Component::Type::LIFESPAN));
        if (!lifeSpanComponent->isAlive())
        {
            e->destroy();
        }
        else
        {
            std::shared_ptr<CRender> renderComponent = std::static_pointer_cast<CRender> (e->getComponentByType(Component::Type::RENDER));
            sf::Color fillColor = renderComponent->m_shape.getFillColor();
            sf::Color outlineColor = renderComponent->m_shape.getOutlineColor();
            fillColor.a -= fillColor.a >= 2.55 ? 2.55 : 0;
            outlineColor.a -= outlineColor.a >= 2.55 ? 2.55 : 0;

            renderComponent->m_shape.setFillColor(fillColor);
            renderComponent->m_shape.setOutlineColor(outlineColor);
            lifeSpanComponent->decreaseTimeToLive();
        }
    }
}

bool LifespanSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}
