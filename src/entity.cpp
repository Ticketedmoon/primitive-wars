#include <iostream>
#include "entity.h"

Entity::Entity(size_t id, Entity::Type type) : m_id(id), type(type), isAlive(true)
{

}

bool Entity::hasComponent(const Component::Type componentType) const
{
    return  m_componentsByType.contains(componentType);
}

std::shared_ptr<Component> Entity::getComponentByType(const Component::Type componentType)
{
    return m_componentsByType[componentType];
}