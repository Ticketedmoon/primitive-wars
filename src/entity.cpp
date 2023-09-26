#include "entity.h"

Entity::Entity(size_t id, Entity::Type type) : m_id(id), type(type), isAlive(true)
{
}

bool Entity::hasComponent(const Component::Type componentType) const
{
    auto index = static_cast<uint8_t>(componentType);
    return m_components[index] != nullptr;
}

std::shared_ptr<Component> Entity::getComponentByType(const Component::Type componentType)
{
    auto index = static_cast<uint8_t>(componentType);
    return m_components[index];
}