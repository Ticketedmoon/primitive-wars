#include <cstdint>
#include "component.h"

static const float DEFAULT_SPEED = 10.0f;

class CTransform : public Component
{
    public:
        Type getType() override;

    public:
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f rotation;
        sf::Vector2f angle;
        float radius;

        float speedX = DEFAULT_SPEED;
        float speedY = DEFAULT_SPEED;
};
