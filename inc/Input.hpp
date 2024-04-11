#pragma once

#include "pch.hpp"

#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "KeyCodes.hpp"

namespace Input {

    // TODO !!! relying on a glfwWindow is bad, this will change !!!
    class InputManager final {
    public:

        inline bool isKeyPressed(const KeyCodes key) const {
            return !previousKeyStates[static_cast<std::size_t>(key)] &&
                currentKeyStates[static_cast<std::size_t>(key)];
        }

        inline bool isKeyReleased(const KeyCodes key) const {
            return previousKeyStates[static_cast<std::size_t>(key)] &&
                !currentKeyStates[static_cast<std::size_t>(key)];
        }

        inline bool isKeyHeld(const KeyCodes key) const {
            return previousKeyStates[static_cast<std::size_t>(key)] &&
                currentKeyStates[static_cast<std::size_t>(key)];
        }

        // TODO needs fixing, the point is not to uuuuuuse glfw here, darnnnn it
        inline void beginFrame(Window::WindowManager* windowManager) {
            previousKeyStates = currentKeyStates;

            for(std::size_t i{ 0 }; i < static_cast<std::size_t>(KeyCodes::KEY_COUNT); ++i) {
                KeyCodes keyCode{ static_cast<KeyCodes>(i) };
                currentKeyStates[i] = glfwGetKey(windowManager->getWindow(), mapKeyCodeToGLFWKey(keyCode)) == GLFW_PRESS;
            }
        }

    private:
        std::array<bool, static_cast<unsigned int>(KeyCodes::KEY_COUNT)> previousKeyStates;
        std::array<bool, static_cast<unsigned int>(KeyCodes::KEY_COUNT)> currentKeyStates;
    };

};
