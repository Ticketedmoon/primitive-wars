#pragma once

#ifndef PRIMITIVE_WARS_AUDIO_MANAGER_H
#define PRIMITIVE_WARS_AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

#include <cassert>
#include <iostream>

#include "action.h"

class AudioManager {
    public:
        ~AudioManager();
        AudioManager(AudioManager& audioManager) = delete;
        void operator=(const AudioManager&) = delete;

        static AudioManager* getInstance();
        void playSound(Action::Type actionType, float volume);
        void playMusic(uint8_t sceneIndex, float volume, bool shouldLoop);

    private:
        AudioManager();

    private:
        static const inline std::string SHOOT_SFX_PATH = "resources/assets/sound/sfx/laser_shoot.wav";
        static const inline std::string SPECIAL_ATTACK_SFX_PATH = "resources/assets/sound/sfx/laser_special_attack.wav";
        static const inline std::string MENU_THEME_PATH = "resources/assets/sound/music/menu_theme.wav";
        static const inline std::string LEVEL_ONE_PATH = "resources/assets/sound/music/level_1.wav";

        static inline AudioManager* m_audioManager;

        static inline std::unordered_map<Action::Type, sf::SoundBuffer> m_soundBufferMap;
        static inline std::unordered_map<Action::Type, sf::Sound> m_soundMap;
        static inline std::vector<sf::Music*> m_sceneMusic;
        static inline uint8_t currentSceneIndex;
};


#endif //PRIMITIVE_WARS_AUDIO_MANAGER_H
