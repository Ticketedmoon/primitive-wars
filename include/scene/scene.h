#pragma once

#ifndef PRIMITIVE_WARS_SCENE_H
#define PRIMITIVE_WARS_SCENE_H

// Forward-declaration
class GameEngine;

class Scene
{
    public:
        enum class Type
        {
            MENU_SCENE,
            GAMEPLAY_SCENE
        };

        explicit Scene(GameEngine& gameEngine);
        virtual ~Scene() = default;

        virtual void update() = 0;
        virtual void render() = 0;
        virtual void performAction() = 0;

    protected:
        GameEngine& gameEngine;
};

#endif //PRIMITIVE_WARS_SCENE_H
