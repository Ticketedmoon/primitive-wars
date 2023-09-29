#pragma once

#ifndef PRIMITIVE_WARS_GUI_SYSTEM_H
#define PRIMITIVE_WARS_GUI_SYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>

#include "c_score.h"
#include "c_render.h"
#include "system.h"
#include "entity/entity_manager.h"
#include "core/common_constants.h"
#include "core/game_properties.h"

class GuiSystem : public System
{
    public:
        explicit GuiSystem(sf::RenderWindow& renderWindow, EntityManager& entityManager, sf::Clock& worldClock,
                GameProperties& gameProperties);
        void execute() override;
        bool shouldApply(GameProperties gameProperties) override;

    private:
        void updateGuiData();
        void drawGuiData();

        void renderCooldownText(std::shared_ptr<Entity>& player);
        void renderTextOnPlayerDeath();
        [[nodiscard]] bool isPlayerWaitingOnRespawnTime() const;

        static void configureTextRendering();
        void drawText(sf::Text& text, const sf::Color& fillColour, uint8_t characterSize, sf::Vector2f position);

    private:
        static const inline std::string FONT_PATH = "resources/fonts/calibri.ttf";
        static const inline sf::String& PAUSED_TEXT = "GAME PAUSED";
        static const inline sf::Vector2f PAUSED_TEXT_SCREEN_POSITION = sf::Vector2f(WINDOW_WIDTH / 2 - 392, WINDOW_HEIGHT / 2 - 96);

        sf::RenderWindow& m_window;
        EntityManager& m_entityManager;
        sf::Clock& m_worldClock;
        GameProperties& m_gameProperties;

        static inline sf::Text gameOverlayText;
        static inline sf::Text respawnText;
        static inline sf::Text pauseText;
        static inline sf::Font m_font;
};


#endif //PRIMITIVE_WARS_GUI_SYSTEM_H
