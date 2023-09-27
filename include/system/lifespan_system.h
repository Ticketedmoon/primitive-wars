#pragma once

#ifndef PRIMITIVE_WARS_LIFESPAN_SYSTEM_H
#define PRIMITIVE_WARS_LIFESPAN_SYSTEM_H

#include <ranges>
#include <memory>

#include "entity.h"
#include "c_lifespan.h"
#include "c_render.h"
#include "system.h"
#include "entity_manager.h"

class LifespanSystem : public System
{
    public:
        explicit LifespanSystem(EntityManager& entityManager);
        void execute() override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        EntityManager& m_entityManager;
};


#endif //PRIMITIVE_WARS_LIFESPAN_SYSTEM_H
