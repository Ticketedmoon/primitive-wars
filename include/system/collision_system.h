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
        CollisionSystem(EntityManager& entityManager, sf::Clock& worldClock, size_t& score,
                float& playerRespawnSeconds, size_t& totalDeaths);

        void execute() override;

    private:

        void checkForWindowCollision(const std::shared_ptr<Entity>& e) const;
        static bool isCollidingAABB(const std::shared_ptr<CRender>& renderComponentForEntity,
                const std::shared_ptr<CRender>& renderComponentForEnemy);

    private:
        EntityManager& m_entityManager;

        sf::Clock& m_worldClock;
        size_t& m_score;
        float& m_playerRespawnSeconds;
        size_t& m_totalDeaths;
};


#endif //PRIMITIVE_WARS_COLLISION_SYSTEM_H
