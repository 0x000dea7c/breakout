#pragma once

#include "pch.hpp"

#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <cstdlib>

namespace SubSystems {

    class SubSystems final {
    public:
        SubSystems()
        {
            if(!glfwInit()) {
                std::cerr << "Couldn't initialize GLFW3.\n";
                exit(EXIT_FAILURE);
            }

            if(SDL_Init(SDL_INIT_AUDIO) < 0) {
                std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << '\n';
                exit(EXIT_FAILURE);
            }

            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                std::cerr << "Couldn't initialize Mix_OpenAudio.\n";
                exit(EXIT_FAILURE);
            }
        }

        ~SubSystems()
        {
            glfwTerminate();
            Mix_CloseAudio();
            SDL_Quit();
        }
    };

};
