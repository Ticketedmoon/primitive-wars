#pragma once

#ifndef PRIMITIVE_WARS_RENDER_SYSTEM_H
#define PRIMITIVE_WARS_RENDER_SYSTEM_H

#include <cmath>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "system.h"
#include "entity/entity_manager.h"
#include "c_render.h"
#include "c_transform.h"
#include "core/common_constants.h"

class RenderSystem : public System
{
    public:
        explicit RenderSystem(sf::RenderWindow& window, EntityManager& entityManager);

        void execute(GameProperties& gameProperties) override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        static void configureTextRendering();
        void drawEntities();
        void drawGuiData(GameProperties& gameProperties);

        void renderTextOnPlayerDeath(GameProperties& gameProperties);
        void renderGameOverlayText(std::shared_ptr<Entity>& player, GameProperties& gameProperties);
        [[nodiscard]] bool isPlayerWaitingOnRespawnTime(GameProperties& gameProperties) const;
        void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize, sf::Vector2f position);

    private:
        static const inline std::string FONT_PATH = "resources/fonts/calibri.ttf";
        static const inline sf::String& PAUSED_TEXT = "GAME PAUSED";
        static const inline sf::Vector2f PAUSED_TEXT_SCREEN_POSITION = sf::Vector2f(WINDOW_WIDTH / 2 - 392, WINDOW_HEIGHT / 2 - 96);

        sf::RenderWindow& m_renderWindow;
        EntityManager& m_entityManager;

        static inline sf::Text m_gameOverlayText;
        static inline sf::Text m_respawnText;
        static inline sf::Text m_pauseText;
        static inline sf::Font m_font;
};


#endif //PRIMITIVE_WARS_RENDER_SYSTEM_H
