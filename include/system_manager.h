#pragma once

#ifndef PRIMITIVE_WARS_SYSTEM_MANAGER_H
#define PRIMITIVE_WARS_SYSTEM_MANAGER_H

#include <ranges>
#include <vector>
#include <memory>
#include "system.h"

class SystemManager
{
    public:
        enum class SystemType
        {
                UPDATE,
                RENDER
        };

        void update(GameProperties& properties);
        void render(GameProperties& properties);

        void registerSystem(std::shared_ptr<System> system, SystemType type);

    private:
        std::vector<std::shared_ptr<System>> m_systemsToUpdate;
        std::vector<std::shared_ptr<System>> m_systemsToRender;
};


#endif //PRIMITIVE_WARS_SYSTEM_MANAGER_H
