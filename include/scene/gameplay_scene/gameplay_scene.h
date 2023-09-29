#pragma once

#ifndef PRIMITIVE_WARS_GAMEPLAY_SCENE_H
#define PRIMITIVE_WARS_GAMEPLAY_SCENE_H

#include "scene/gameplay_scene/system/collision_system.h"
#include "scene/gameplay_scene/system/entity_spawn_system.h"
#include "scene/gameplay_scene/system/lifespan_system.h"
#include "scene/gameplay_scene/system/transform_system.h"
#include "scene/gameplay_scene/system/render_system.h"
#include "scene/gameplay_scene/system/gui_system.h"
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
        static void registerSystems(GameEngine& engine);

    private:
        static const inline std::string BACKGROUND_IMAGE_PATH = "resources/assets/board.png";

        static inline SystemManager m_systemManager;
        static inline EntityManager m_entityManager;

        static inline sf::Texture textureSprite;
        static inline sf::Sprite backgroundSprite;
};


#endif //PRIMITIVE_WARS_GAMEPLAY_SCENE_H
