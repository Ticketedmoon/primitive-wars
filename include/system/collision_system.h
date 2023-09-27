#pragma once

#ifndef PRIMITIVE_WARS_COLLISION_SYSTEM_H
#define PRIMITIVE_WARS_COLLISION_SYSTEM_H

#include <vector>
#include <memory>

#include "system.h"
#include "entity_manager.h"
#include "c_render.h"
#include "entity.h"
#include "c_render.h"
#include "c_transform.h"
#include "c_collision.h"
#include "entity_spawn_system.h"

class CollisionSystem : public System
{
    public:
        explicit CollisionSystem(EntityManager& entityManager);

        void execute() override;

    private:

        static void checkForWindowCollision(const std::shared_ptr<Entity>& e);
        static bool isCollidingAABB(const std::shared_ptr<CRender>& renderComponentForEntity,
                const std::shared_ptr<CRender>& renderComponentForEnemy);

    private:
        EntityManager& m_entityManager;
};


#endif //PRIMITIVE_WARS_COLLISION_SYSTEM_H
