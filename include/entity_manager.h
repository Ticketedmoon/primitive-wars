#pragma once

#ifndef PRIMITIVE_WARS_ENTITY_MANAGER_H
#define PRIMITIVE_WARS_ENTITY_MANAGER_H


#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>
#include "entity.h"

class EntityManager
{
    public:
        void update();
        std::shared_ptr<Entity>& addEntity(Entity::Type type);
        std::shared_ptr<Entity>&  removeEntity(size_t id);
        std::vector<std::shared_ptr<Entity>>& getEntities();
        std::vector<std::shared_ptr<Entity>>& getEntitiesByType(Entity::Type type);

    private:
        size_t m_totalEntities;
        std::vector<std::shared_ptr<Entity>> m_entities;
        std::vector<std::shared_ptr<Entity>> m_entitiesToAdd; // Holds enemies to add each frame, will be added to m_entities on next frame update
        std::unordered_map<Entity::Type, std::vector<std::shared_ptr<Entity>>> m_entitiesByType;

};


#endif //PRIMITIVE_WARS_ENTITY_MANAGER_H
