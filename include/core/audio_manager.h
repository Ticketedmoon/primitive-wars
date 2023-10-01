#pragma once

#ifndef PRIMITIVE_WARS_AUDIO_MANAGER_H
#define PRIMITIVE_WARS_AUDIO_MANAGER_H

#include <SFML/Audio.hpp>

#include <cassert>
#include <iostream>

#include "action.h"

class AudioManager {
    public:
        enum class AudioType
        {
            SHOOT = 0,
            SPECIAL_ATTACK = 1,
            GAME_OVER = 2
        };

        ~AudioManager();
        AudioManager(AudioManager& audioManager) = delete;
        void operator=(const AudioManager&) = delete;

        static AudioManager* getInstance();
        void playSound(AudioType audioType, float volume);
        void playMusic(uint8_t sceneIndex, float volume, bool shouldLoop);
        bool isMusicPlaying();
        void stopActiveMusic();
        sf::Time getCurrentMusicDuration();

    private:
        AudioManager();

    private:
        static const inline std::string SHOOT_SFX_PATH = "resources/assets/sound/sfx/laser_shoot.wav";
        static const inline std::string SPECIAL_ATTACK_SFX_PATH = "resources/assets/sound/sfx/laser_special_attack.wav";
        static const inline std::string GAME_OVER_SFX_PATH = "resources/assets/sound/sfx/game_over.wav";

        static const inline std::string MENU_THEME_PATH = "resources/assets/sound/music/menu_theme.wav";
        static const inline std::string LEVEL_ONE_PATH = "resources/assets/sound/music/level_1.wav";
        static const inline std::string LEVEL_TWO_PATH = "resources/assets/sound/music/level_2.wav";
        static const inline std::string LEVEL_THREE_PATH = "resources/assets/sound/music/level_3.wav";

        static inline AudioManager* m_audioManager;

        static inline std::unordered_map<AudioType, sf::SoundBuffer> m_soundBufferMap;
        static inline std::unordered_map<AudioType, sf::Sound> m_soundMap;
        static inline std::vector<sf::Music*> m_sceneMusic;
        static inline uint8_t currentSceneIndex;
};


#endif //PRIMITIVE_WARS_AUDIO_MANAGER_H
