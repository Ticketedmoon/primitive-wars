#pragma once

#ifndef PRIMITIVE_WARS_TRANSFORM_SYSTEM_H
#define PRIMITIVE_WARS_TRANSFORM_SYSTEM_H

#include <vector>
#include <memory>

#include "entity.h"
#include "entity_manager.h"
#include "c_transform.h"
#include "c_collision.h"
#include "c_user_input.h"
#include "system.h"

class TransformSystem : public System
{
    public:
        explicit TransformSystem(EntityManager& entityManager);

        void execute() override;

    private:
        static void updateEntityTransformByUserInput(const std::shared_ptr<Entity>& e,
                std::shared_ptr<CTransform>& transformComponent,
                const std::shared_ptr<CCollision>& collisionComponent);
        static void updateTransformOnWindowBorderCollision(std::shared_ptr<CTransform>& transformComponent,
                const std::shared_ptr<CCollision>& collisionComponent) ;

    private:
        EntityManager& m_entityManager;
};


#endif //PRIMITIVE_WARS_TRANSFORM_SYSTEM_H
