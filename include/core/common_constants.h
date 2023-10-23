#pragma once

#ifndef PRIMITIVE_WARS_COMMON_CONSTANTS_H
#define PRIMITIVE_WARS_COMMON_CONSTANTS_H

#include <cstdint>

static const uint32_t WINDOW_WIDTH = 1280;
static const uint32_t WINDOW_HEIGHT = 720;
static const float DEFAULT_RESPAWN_RATE_SECONDS = 3.0f;

struct LevelClearStatus
{
    bool levelOneCleared;
    bool levelTwoCleared;
    bool levelThreeCleared;
};

#endif //PRIMITIVE_WARS_COMMON_CONSTANTS_H
