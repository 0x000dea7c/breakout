#include "Audio.hpp"

#include <cstdlib>
#include <iostream>

namespace Audio {

    AudioManager::AudioManager()
        : volume{ MIX_MAX_VOLUME }
    {
        // start the game with full volumeZzZ
        Mix_MasterVolume(static_cast<int>(volume));
        Mix_VolumeMusic(static_cast<int>(volume));

        // load all game's music shiat herE
        // if smthing goes wrong, ex1t cuz you can't play w/o music
        // let's go biaaaaatch
        if(!loadFromFile("./res/audio/breakout.mp3",
                         AudioID_bgMusic,
                         Audio::AudioType::MUSIC)) {
            std::cerr << "Couldn't load breakout.mp3\n";
            exit(EXIT_FAILURE);
        }

        if(!loadFromFile("./res/audio/ballHitPlayer.wav",
                         AudioID_bhitPlayer,
                         Audio::AudioType::CHUNK)) {
            std::cerr << "Couldn't load ballHitPlayer.wav\n";
            exit(EXIT_FAILURE);
        }

        if(!loadFromFile("./res/audio/ballHitSolidBrick.wav",
                         AudioID_bhitSolidBrick,
                         Audio::AudioType::CHUNK)) {
            std::cerr << "Couldn't load ballHitSolidBrick.wav\n";
            exit(EXIT_FAILURE);
        }

        if(!loadFromFile("./res/audio/powerUp.wav",
                         AudioID_powerUp,
                         Audio::AudioType::CHUNK)) {
            std::cerr << "Couldn't load powerUp.wav\n";
            exit(EXIT_FAILURE);
        }

        if(!loadFromFile("./res/audio/ballHitNonSolidBrick.wav",
                         AudioID_bhitNonSolidBrick,
                         Audio::AudioType::CHUNK)) {
            std::cerr << "Couldn't load ballHitNonSolidBrick.wav\n";
            exit(EXIT_FAILURE);
        }

        if(!loadFromFile("./res/audio/roundWin.wav",
                         AudioID_roundWin,
                         Audio::AudioType::CHUNK)) {
            std::cerr << "Couldn't load roundWin.wav\n";
            exit(EXIT_FAILURE);
        }

        if(!loadFromFile("./res/audio/winGame.wav",
                         AudioID_winGame,
                         Audio::AudioType::CHUNK)) {
            std::cerr << "Couldn't load winGame.wav\n";
            exit(EXIT_FAILURE);
        }
    }

    AudioManager::~AudioManager()
    {
        for(const auto& m : music) {
            Mix_FreeMusic(m.second);
        }

        for(const auto& c : chunks) {
            Mix_FreeChunk(c.second);
        }
    }

    bool AudioManager::loadFromFile(const std::filesystem::path& path,
                                    const std::string& id,
                                    const AudioType type)
    {
        // play me, biatch
        if(type == AudioType::MUSIC) {
            Mix_Music* ptr = Mix_LoadMUS(path.c_str());
            if(!ptr) {
                return false;
            }
            music[id] = ptr;
        } else if(type == AudioType::CHUNK) {
            Mix_Chunk* ptr = Mix_LoadWAV(path.c_str());
            if(!ptr) {
                return false;
            }
            chunks[id] = ptr;
        }

        return true;
    }

    void AudioManager::playMusic(const std::string& id)
    {
        Mix_PlayMusic(music.at(id), -1);
    }

    void AudioManager::playChunk(const std::string& id)
    {
        Mix_PlayChannel(-1, chunks.at(id), 0);
    }

    void AudioManager::increaseVolume()
    {
        volume = std::min(volume + 13.3f, static_cast<float>(MIX_MAX_VOLUME));
        Mix_MasterVolume(static_cast<int>(volume));
        Mix_VolumeMusic(static_cast<int>(volume));
    }

    void AudioManager::decreaseVolume()
    {
        volume = std::max(volume - 13.3f, 0.f);
        Mix_MasterVolume(static_cast<int>(volume));
        Mix_VolumeMusic(static_cast<int>(volume));
    }
};
