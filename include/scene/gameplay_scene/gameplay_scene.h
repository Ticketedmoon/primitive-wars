#pragma once

#ifndef PRIMITIVE_WARS_GAMEPLAY_SCENE_H
#define PRIMITIVE_WARS_GAMEPLAY_SCENE_H

#include "scene/gameplay_scene/system/collision_system.h"
#include "scene/gameplay_scene/system/entity_spawn_system.h"
#include "scene/gameplay_scene/system/lifespan_system.h"
#include "scene/gameplay_scene/system/transform_system.h"
#include "scene/gameplay_scene/system/render_system.h"
#include "scene/gameplay_scene/system/gui_system.h"
#include "scene/game_over_scene/game_over_scene.h"
#include "system_manager.h"

#include "game_engine.h"
#include "scene.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems(GameEngine& engine);

    private:
        const std::string BACKGROUND_IMAGE_PATH = "resources/assets/texture/board.png";
        sf::Clock levelClock;
        GameProperties gameProperties;

        SystemManager m_systemManager;
        EntityManager m_entityManager;

        sf::Texture textureSprite;
        sf::Sprite backgroundSprite;
};


#endif //PRIMITIVE_WARS_GAMEPLAY_SCENE_H
