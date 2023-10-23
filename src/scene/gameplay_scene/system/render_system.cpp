#include "scene/gameplay_scene/system/render_system.h"

RenderSystem::RenderSystem(sf::RenderWindow& window, EntityManager& entityManager)
    : m_renderWindow(window), m_entityManager(entityManager)
{
}

void RenderSystem::execute(GameProperties& gameProperties)
{
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
}

bool RenderSystem::shouldApply(GameProperties gameProperties)
{
    return true;
}
