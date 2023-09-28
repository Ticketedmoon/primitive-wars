#pragma once

#ifndef PRIMITIVE_WARS_GAMEPLAY_SCENE_H
#define PRIMITIVE_WARS_GAMEPLAY_SCENE_H

#include "user_input_system.h"
#include "collision_system.h"
#include "entity_spawn_system.h"
#include "lifespan_system.h"
#include "transform_system.h"
#include "render_system.h"
#include "gui_system.h"
#include "system_manager.h"

#include "game_engine.h"
#include "scene.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction() override;

    private:
        static const inline std::string BACKGROUND_IMAGE_PATH = "resources/assets/board.png";

        static inline SystemManager m_systemManager;
        static inline EntityManager m_entityManager;

        static inline sf::Texture textureSprite;
        static inline sf::Sprite backgroundSprite;
};


#endif //PRIMITIVE_WARS_GAMEPLAY_SCENE_H
