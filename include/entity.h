#pragma once

#ifndef PRIMITIVE_WARS_ENTITY_H
#define PRIMITIVE_WARS_ENTITY_H

#include <ranges>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include "component/component.h"

const uint8_t TOTAL_COMPONENTS = static_cast<uint8_t>(Component::Type::COUNT);

class Entity
{
    public:
        friend class EntityManager;

        [[nodiscard]] bool hasComponent(Component::Type componentType) const;

        std::shared_ptr<Component> getComponentByType(Component::Type componentType);

    public:
        enum class Type
        {
            PLAYER,
            ENEMY,
            BULLET,
            NONE
        };

        Type type;
        bool isAlive;

        std::array<std::shared_ptr<Component>, TOTAL_COMPONENTS> m_components;

    private:
        Entity(size_t id, Type type);

    private:
        size_t m_id;

};


#endif //PRIMITIVE_WARS_ENTITY_H
