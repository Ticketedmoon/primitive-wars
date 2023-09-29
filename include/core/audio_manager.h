#pragma once

#ifndef PRIMITIVE_WARS_AUDIO_MANAGER_H
#define PRIMITIVE_WARS_AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

#include <cassert>

#include "action.h"

// TODO make me a singleton
class AudioManager {
    public:
        AudioManager();
        ~AudioManager();

        void playSound(Action::Type actionType, float volume);
        void playMusic(uint8_t sceneIndex, float volume);

    private:
        static const inline std::string SHOOT_SFX_PATH = "resources/assets/sound/sfx/laser_shoot.wav";
        static const inline std::string MENU_THEME_PATH = "resources/assets/sound/music/menu_theme.wav";
        static const inline std::string LEVEL_ONE_PATH = "resources/assets/sound/music/level_1.wav";

        sf::SoundBuffer m_shootLaserSoundBuffer;

        std::unordered_map<Action::Type, sf::Sound> m_soundMap;
        std::vector<sf::Music> m_sceneMusic;

        // FIXME temporary
        sf::Music menuTheme;
        sf::Music levelOneTheme;
};


#endif //PRIMITIVE_WARS_AUDIO_MANAGER_H
