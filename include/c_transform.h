#include "component.h"

class CTransform : public Component
{
    public:
        Type getType() override;

    public:
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f rotation;
        sf::Vector2f angle;
};
