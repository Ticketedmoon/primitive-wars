#pragma once

#ifndef PRIMITIVE_WARS_RENDER_SYSTEM_H
#define PRIMITIVE_WARS_RENDER_SYSTEM_H

#include <cmath>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "system.h"
#include "entity_manager.h"
#include "c_render.h"
#include "c_transform.h"
#include "common_constants.h"

struct RenderProperties
{
    float& specialAttackCooldownSeconds;
    size_t& score;
    size_t& totalDeaths;
    float& playerRespawnTimeSeconds;
};

class RenderSystem : public System
{
    public:
        explicit RenderSystem(sf::RenderWindow& window, EntityManager& entityManager, sf::Clock& worldClock,
                float& specialAttackCoolDownSeconds, size_t& score, size_t& totalDeaths,
                float& playerRespawnTimeSeconds, bool& hasPaused);

        void execute() override;

    private:
        static void configureTextRendering();
        void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize, sf::Vector2f position);

    private:
        static const inline std::string FONT_PATH = "resources/fonts/calibri.ttf";
        static const inline sf::String& PAUSED_TEXT = "GAME PAUSED";
        static const inline std::string BACKGROUND_IMAGE_PATH = "resources/assets/board.png";
        static const inline sf::Vector2f CENTRE_SCREEN_POSITION = sf::Vector2f(WINDOW_WIDTH / 2 - 496, WINDOW_HEIGHT / 2 - 96);

        sf::RenderWindow& m_renderWindow;
        EntityManager& m_entityManager;
        sf::Clock& m_worldClock;

        float& m_specialAttackCoolDownSeconds;
        size_t& m_score;
        size_t& m_totalDeaths;
        float& m_playerRespawnTimeSeconds;
        bool& hasPaused;

        static inline sf::Texture textureSprite;
        static inline sf::Sprite backgroundSprite;
        static inline sf::Text gameOverlayText;
        static inline sf::Text respawnText;
        static inline sf::Text pauseText;
        static inline sf::Font m_font;
};


#endif //PRIMITIVE_WARS_RENDER_SYSTEM_H
