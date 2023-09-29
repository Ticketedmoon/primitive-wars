#include "c_score.h"

CScore::CScore(size_t score) : m_score(score)
{

}

size_t CScore::getScore()
{
    return m_score;
}

void CScore::addToScore(size_t score)
{
    m_score += score;
}