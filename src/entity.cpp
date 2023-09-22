#include "entity.h"

Entity::Entity(size_t id, Entity::Type type) : m_id(id), m_type(type), m_alive(true)
{

}

bool Entity::hasComponent(Component::Type type) const
{
    return m_componentsByType.contains(type);
}

std::shared_ptr<Component> Entity::getComponentByType(Component::Type type) const
{
    std::shared_ptr<Component> component = m_componentsByType.at(type);
    return component;
}