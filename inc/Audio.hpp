#pragma once

#include "pch.hpp"

#include <SDL2/SDL_mixer.h>

#include <unordered_map>
#include <filesystem>
#include <string>
#include <cmath>

namespace Audio {

    // TODO yep, yep, yep, yEEEEEEEEEEp
    static const std::string AudioID_bgMusic{ "background" };
    static const std::string AudioID_bhitPlayer{ "ballHitPlayer" };
    static const std::string AudioID_bhitSolidBrick{ "ballHitSolidBrick" };
    static const std::string AudioID_powerUp{ "powerUp" };
    static const std::string AudioID_bhitNonSolidBrick{ "ballHitNonSolidBrick" };
    static const std::string AudioID_roundWin{ "roundWin" };
    static const std::string AudioID_winGame{ "winGame" };

    enum class AudioType {
        MUSIC,
        CHUNK // ChUnKZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
    };

    class AudioManager final {
    public:
        AudioManager();
        ~AudioManager();

        bool loadFromFile(const std::filesystem::path& path,
                          const std::string& id,
                          const AudioType type);

        void playMusic(const std::string& id);
        void playChunk(const std::string& id);
        void increaseVolume();
        void decreaseVolume();

        inline float getNormalizedVolume() const
        {
            // 😴
            return std::round((volume / MIX_MAX_VOLUME) * 10.f) / 10.f;
        }

    private:
        std::unordered_map<std::string, Mix_Music*> music;
        std::unordered_map<std::string, Mix_Chunk*> chunks;
        float volume;
    };

};
