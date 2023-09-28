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
                PAUSE = 6
        };

    private:
        Type type;

};


#endif //PRIMITIVE_WARS_ACTION_H
