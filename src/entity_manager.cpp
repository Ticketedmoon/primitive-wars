#include "entity_manager.h"

void EntityManager::update()
{
    for (const std::shared_ptr<Entity>& e : m_entitiesToAdd)
    {
        m_entities.emplace_back(e);
        m_entitiesByType[e->type].emplace_back(e);
    }

    removeDeadEntities(m_entities);

    for (auto& item: m_entitiesByType)
    {
        removeDeadEntities(item.second);
    }

    m_entitiesToAdd.clear();
}

std::shared_ptr<Entity>& EntityManager::addEntity(Entity::Type type)
{
    size_t entityId = m_totalEntities++;
    std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(entityId, type));
    m_entitiesToAdd.emplace_back(entity);
    return m_entitiesToAdd.back();
}

void EntityManager::removeDeadEntities(std::vector<std::shared_ptr<Entity>>& entities)
{
    const auto& callback = std::remove_if(
            entities.begin(),
            entities.end(),
            [](std::shared_ptr<Entity>& entity) {
                return !entity->isAlive;
            });
    entities.erase(callback, entities.end());
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
    return m_entities;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntitiesByType(Entity::Type type)
{
    return m_entitiesByType[type];
}

std::shared_ptr<Entity>& EntityManager::getEntityByType(Entity::Type type)
{
    return m_entitiesByType[type].front();
}
