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

        [[nodiscard]] bool hasComponent(Component::Type type) const;

        std::shared_ptr<Component> getComponentByType(Component::Type type) const;

    public:
        enum class Type
        {
                PLAYER,
                ENEMY
        };

        std::unordered_map<Component::Type, std::shared_ptr<Component>> m_componentsByType;

    private:
        Entity(size_t id, Type type);

        size_t m_id;
        Type m_type;
        bool m_alive;
};


#endif //PRIMITIVE_WARS_ENTITY_H
