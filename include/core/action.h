#pragma once

#ifndef PRIMITIVE_WARS_ACTION_H
#define PRIMITIVE_WARS_ACTION_H

class Action
{
    public:
        enum class Type
        {
                MOVE_LEFT = 0,
                MOVE_RIGHT = 1,
                MOVE_UP = 2,
                MOVE_DOWN = 3,
                SHOOT = 4,
                SPECIAL_ATTACK = 5,
                SELECT = 6,
                CURSOR_MOVE = 7,
                CURSOR_SELECT = 8,
                PAUSE = 9,
                EXIT_SCENE = 10
        };
        enum class Mode
        {
                PRESS,
                RELEASE
        };

        Action(Type type, Mode mode);
        Type getType();
        Mode getMode();

    private:
        Type m_type;
        Mode m_mode;

};


#endif //PRIMITIVE_WARS_ACTION_H
