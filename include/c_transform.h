#include <cstdint>
#include "component.h"

class CTransform : public Component
{
    public:
        Type getType() override;

    public:
        sf::Vector2f position;
        sf::Vector2f speed;

        // TODO temporary
        sf::Vector2f speedDelta;
};
