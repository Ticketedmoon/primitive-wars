#pragma once

#ifndef PRIMITIVE_WARS_ENTITY_H
#define PRIMITIVE_WARS_ENTITY_H

#include <ranges>
#include <cstddef>
#include <vector>
#include <memory>
#include <unordered_map>
#include "component.h"

class Entity
{
    public:
        friend class EntityManager;

        [[nodiscard]] bool hasComponent(const Component::Type componentType) const;

        std::shared_ptr<Component> getComponentByType(const Component::Type componentType);

        enum class Type
        {
            PLAYER,
            ENEMY,
            BULLET
        };

        Type type;
        bool isAlive;

    public:
        std::unordered_map<Component::Type, std::shared_ptr<Component>> m_componentsByType;

    private:
        Entity(size_t id, Type type);

        size_t m_id;
};


#endif //PRIMITIVE_WARS_ENTITY_H
