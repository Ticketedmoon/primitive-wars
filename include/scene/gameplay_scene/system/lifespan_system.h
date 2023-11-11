#pragma once

#ifndef PRIMITIVE_WARS_LIFESPAN_SYSTEM_H
#define PRIMITIVE_WARS_LIFESPAN_SYSTEM_H

#include <ranges>
#include <memory>

#include "entity/entity.h"
#include "c_lifespan.h"
#include "c_render.h"
#include "system.h"
#include "entity/entity_manager.h"

class LifespanSystem : public System
{
    public:
        explicit LifespanSystem(EntityManager& entityManager);
        void execute(GameProperties& gameProperties) override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        static constexpr sf::Uint8 LIFESPAN_FADE_INCREMENT = sf::Uint8(2.25f);

        EntityManager& m_entityManager;
};


#endif //PRIMITIVE_WARS_LIFESPAN_SYSTEM_H
