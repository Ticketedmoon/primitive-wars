#include "entity_manager.h"

void EntityManager::update()
{
    for (const std::shared_ptr<Entity>& e : m_entitiesToAdd)
    {
        m_entities.emplace_back(e);
        m_entitiesByType[e->m_type].emplace_back(e);
    }

    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity>& entity) {
        return !entity->m_alive;
    }), m_entities.end());

    for (auto item: m_entitiesByType)
    {
        item.second.erase(std::remove_if(item.second.begin(), item.second.end(), [](std::shared_ptr<Entity>& entity) {
            return !entity->m_alive;
        }), item.second.end());
    }

    m_entitiesToAdd.clear();
}

std::shared_ptr<Entity>& EntityManager::addEntity(Entity::Type type)
{
    size_t entityId = m_totalEntities++;
    std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(entityId, type));
    m_entitiesToAdd.emplace_back(entity);
    return m_entitiesToAdd.at(entityId);
}

std::shared_ptr<Entity>& EntityManager::removeEntity(size_t id)
{
    std::shared_ptr<Entity>& entity = m_entities.at(id);
    entity->m_alive = false;
    return entity;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
    return m_entities;
}

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntitiesByType(Entity::Type type)
{
    return m_entitiesByType[type];
}
