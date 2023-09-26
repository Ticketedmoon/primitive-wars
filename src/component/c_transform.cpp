#include "c_transform.h"

CTransform::CTransform(sf::Vector2f position, sf::Vector2f speed) : m_position(position), m_speed(speed)
{

}

CTransform::CTransform(sf::Vector2f position, sf::Vector2f speed, sf::Vector2f speedDelta)
    : m_position(position), m_speed(speed), m_speedDelta(speedDelta)
{

}
