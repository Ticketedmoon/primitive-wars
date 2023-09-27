#include "system_manager.h"

void SystemManager::update(GameProperties& properties)
{
    std::ranges::filter_view view = m_systemsToUpdate | std::ranges::views::filter([properties](std::shared_ptr<System>& s) {
        return s->shouldApply(properties);
    });
    std::vector<std::shared_ptr<System>> systemsToUpdate = std::vector(view.begin(), view.end());
    for (std::shared_ptr<System>& system : systemsToUpdate)
    {
        system->execute();
    }
}

void SystemManager::render(GameProperties& properties)
{
    std::ranges::filter_view view = m_systemsToRender | std::ranges::views::filter([properties](std::shared_ptr<System>& s) {
        return s->shouldApply(properties);
    });
    std::vector<std::shared_ptr<System>> systemsToUpdate = std::vector(view.begin(), view.end());
    for (std::shared_ptr<System>& system : systemsToUpdate)
    {
        system->execute();
    }
}

void SystemManager::registerSystem(std::shared_ptr<System> system, SystemType type)
{
    if (type == SystemType::UPDATE)
    {
        m_systemsToUpdate.emplace_back(system);
    }
    else if (type == SystemType::RENDER)
    {
        m_systemsToRender.emplace_back(system);
    }
}
