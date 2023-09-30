#include "audio_manager.h"

AudioManager::AudioManager()
{
    assert(soundBuffer.loadFromFile(SHOOT_SFX_PATH));
    m_soundMap[Action::Type::SHOOT] = sf::Sound(soundBuffer);

    auto* menuTheme = new sf::Music();
    auto* levelOneTheme = new sf::Music();
    assert(menuTheme->openFromFile(MENU_THEME_PATH));
    assert(levelOneTheme->openFromFile(LEVEL_ONE_PATH));

    m_sceneMusic.emplace_back(menuTheme);
    m_sceneMusic.emplace_back(levelOneTheme);
}

AudioManager::~AudioManager()
{
    for (sf::Music* music : m_sceneMusic)
    {
        delete music;
    }

    delete m_audioManager;
    m_sceneMusic.clear();
}

AudioManager* AudioManager::getInstance()
{
    if (m_audioManager != nullptr)
    {
        return m_audioManager;
    }

    m_audioManager = new AudioManager();
    currentSceneIndex = static_cast<uint8_t>(Scene::Type::MENU_SCENE);

    return m_audioManager;
}

void AudioManager::playSound(Action::Type actionType, float volume)
{
    sf::Sound& sound = m_soundMap[actionType];
    sound.setVolume(volume);
    sound.play();
}

void AudioManager::playMusic(uint8_t sceneIndex, float volume, bool shouldLoop)
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    currentMusic.stop();

    sf::Music& nextMusic = *m_sceneMusic[sceneIndex];
    nextMusic.setVolume(volume);
    nextMusic.setLoop(shouldLoop);
    nextMusic.play();

    currentSceneIndex = sceneIndex;
}