#pragma once

#include "pch.hpp"

#include "Event.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextRenderer.hpp"
#include "Window.hpp"
#include "Audio.hpp"

#include <array>

namespace Breakout {

    enum class MenuItem {
        PLAY,
        SOUND,
        QUIT
    };

    // yep, convenient to wRaP aRouNdDdDd
    inline MenuItem& operator++(MenuItem& m)
    {
        switch(m) {
        case MenuItem::PLAY:
            return m = MenuItem::SOUND;
        case MenuItem::SOUND:
            return m = MenuItem::QUIT;
        case MenuItem::QUIT:
            return m = MenuItem::PLAY;
        }
    }

    // aDdiNg compLexity
    inline MenuItem& operator--(MenuItem& m)
    {
        switch(m) {
        case MenuItem::PLAY:
            return m = MenuItem::QUIT;
        case MenuItem::SOUND:
            return m = MenuItem::PLAY;
        case MenuItem::QUIT:
            return m = MenuItem::SOUND;
        }
    }

    class Menu final {
    public:
        Menu(Event::EventManager* aEventManager,
             Input::InputManager* aInputManager,
             const Resource::Shader* aShader,
             const Resource::Texture* aTexture,
             Renderer::TextRenderer* aTextRenderer,
             Window::WindowManager* aWindowManager,
             Audio::AudioManager* aAudioManager);

        ~Menu();

        void processInput();
        void render() const;
    private:
        Event::EventManager* eventManager;
        Input::InputManager* inputManager;
        const Resource::Shader* shader;
        const Resource::Texture* texture;
        Renderer::TextRenderer* textRenderer;
        Window::WindowManager* windowManager;
        Audio::AudioManager* audioManager;
        MenuItem currentItem;
        unsigned int VAO;
    };

};
