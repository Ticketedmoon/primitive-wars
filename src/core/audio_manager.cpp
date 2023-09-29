#include "audio_manager.h"

AudioManager::AudioManager() {
    assert(m_shootLaserSoundBuffer.loadFromFile(SHOOT_SFX_PATH));
    m_soundMap[Action::Type::SHOOT] = sf::Sound(m_shootLaserSoundBuffer);

    assert(menuTheme.openFromFile(MENU_THEME_PATH));
    assert(levelOneTheme.openFromFile(LEVEL_ONE_PATH));
}

AudioManager::~AudioManager()
{
    m_sceneMusic.clear();
}

void AudioManager::playSound(Action::Type actionType, float volume) {
    sf::Sound& sound = m_soundMap[actionType];
    sound.setVolume(volume);
    sound.play();
}

void AudioManager::playMusic(uint8_t sceneIndex, float volume) {

    if (sceneIndex == 0)
    {
        levelOneTheme.stop();
        menuTheme.setVolume(volume);
        menuTheme.play();
    }
    else if (sceneIndex == 1)
    {
        menuTheme.stop();
        levelOneTheme.setVolume(volume);
        levelOneTheme.play();
    }
}
