#include "audio_manager.h"

AudioManager::AudioManager()
{
    m_soundBufferMap[AudioType::SHOOT] = sf::SoundBuffer();
    m_soundBufferMap[AudioType::SPECIAL_ATTACK] = sf::SoundBuffer();
    m_soundBufferMap[AudioType::GAME_OVER] = sf::SoundBuffer();

    m_soundMap[AudioType::SHOOT] = sf::Sound(m_soundBufferMap[AudioType::SHOOT]);
    m_soundMap[AudioType::SPECIAL_ATTACK] = sf::Sound(m_soundBufferMap[AudioType::SPECIAL_ATTACK]);
    m_soundMap[AudioType::GAME_OVER] = sf::Sound(m_soundBufferMap[AudioType::GAME_OVER]);
    assert(m_soundBufferMap[AudioType::SHOOT].loadFromFile(SHOOT_SFX_PATH));
    assert(m_soundBufferMap[AudioType::SPECIAL_ATTACK].loadFromFile(SPECIAL_ATTACK_SFX_PATH));
    assert(m_soundBufferMap[AudioType::GAME_OVER].loadFromFile(GAME_OVER_SFX_PATH));

    // TODO Consider loading these when we need them - lazy loading.
    auto* menuTheme = new sf::Music();
    auto* levelOneTheme = new sf::Music();
    auto* levelTwoTheme = new sf::Music();
    auto* levelThreeTheme = new sf::Music();

    assert(menuTheme->openFromFile(MENU_THEME_PATH));
    assert(levelOneTheme->openFromFile(LEVEL_ONE_PATH));
    assert(levelTwoTheme->openFromFile(LEVEL_TWO_PATH));
    assert(levelThreeTheme->openFromFile(LEVEL_THREE_PATH));

    m_sceneMusic.emplace_back(menuTheme);
    m_sceneMusic.emplace_back(menuTheme);
    m_sceneMusic.emplace_back(levelOneTheme);
    m_sceneMusic.emplace_back(levelTwoTheme);
    m_sceneMusic.emplace_back(levelThreeTheme);
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
    currentSceneIndex = 0;

    return m_audioManager;
}

void AudioManager::playSound(AudioType actionType, float volume)
{
    sf::Sound& sound = m_soundMap[actionType];
    sound.setVolume(volume);
    sound.play();
}

void AudioManager::playMusic(uint8_t sceneIndex, float volume, bool shouldLoop)
{
    stopActiveMusic();

    sf::Music& nextMusic = *m_sceneMusic[sceneIndex];
    nextMusic.setVolume(volume);
    nextMusic.setLoop(shouldLoop);
    nextMusic.play();

    currentSceneIndex = sceneIndex;
}

bool AudioManager::isMusicPlaying()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    return currentMusic.getStatus() == sf::SoundSource::Playing;
}

void AudioManager::stopActiveMusic()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    currentMusic.stop();
}

sf::Time AudioManager::getCurrentMusicDuration()
{
    sf::Music& currentMusic = *m_sceneMusic[currentSceneIndex];
    return currentMusic.getDuration();
}