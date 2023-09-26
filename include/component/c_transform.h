#include <cstdint>
#include "component.h"

class CTransform : public Component
{
    public:
        CTransform(sf::Vector2f position, sf::Vector2f speed);
        CTransform(sf::Vector2f position, sf::Vector2f speed, sf::Vector2f speedDelta);

    public:
        sf::Vector2f m_position;
        sf::Vector2f m_speed;
        sf::Vector2f m_speedDelta;
};