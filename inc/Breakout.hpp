#pragma once

#include "pch.hpp"

#include "Window.hpp"
#include "Input.hpp"
#include "GameState.hpp"
#include "Event.hpp"
#include "Menu.hpp"
#include "Audio.hpp"
#include "Texture.hpp"
#include "SpriteRenderer.hpp"
#include "Ball.hpp"
#include "PowerUps.hpp"
#include "Background.hpp"
#include "Bricks.hpp"
#include "ParticleEmitter.hpp"
#include "PostProcessor.hpp"
#include "UIRenderer.hpp"
#include "Shader.hpp"

#include <array>
#include <filesystem>

namespace Breakout {
    // player shit
    static constexpr unsigned int TotalPlayerLives{ 3 };

    class Game final {
    public:
        Game(Window::WindowManager* aWindowManager,
             Input::InputManager* aInputManager,
             Event::EventManager* aEventManager,
             Menu* aMenu,
             Audio::AudioManager* aAudioManager,
             Resource::TextureManager* aTextureManager,
             Renderer::SpriteRenderer* aSpriteRenderer,
             Effect::ParticleEmitter* aParticleEmitter,
             Effect::PostProcessor* aPostProcessor,
             Renderer::TextRenderer* aTextRenderer,
             Renderer::UIRenderer* aUIRenderer,
             Resource::ShaderManager *aShaderManager);

        void run();

    private:
        void processInput();
        void update(const float deltaTime);
        void render() const;

        void playGame();
        void quitGame();

        void initializePlayer();
        void updatePlayer(const float deltaTime);

        void initializeBall();
        void updateBall(const float deltaTime);

        void initializePowerUps();
        void updatePowerUps(const float deltaTime);
        void clearPowerUps();
        void spawnPowerUp(const glm::vec2& referencePosition);
        void reActivatePowerUp(const std::size_t index);
        void removePowerUpEffects();
        void addPowerUp(const PowerUpType type,
                        const glm::vec3& colour,
                        const glm::vec2& position,
                        const glm::vec2& velocity,
                        const float duration,
                        const int width,
                        const int height,
                        const Resource::Texture* texture);
        std::size_t getNextPowerUpIndex() const;

        inline bool powerUpShouldSpawn(const unsigned int chance)
        {
            const unsigned int random{ std::rand() % chance };
            return random == 0;
        }

        void initializeBackground();

        void initializeLevels();
        bool loadGameLevelFromFile(const std::filesystem::path& filePath,
                                   const std::size_t levelIndex);

        void updateCollisions();

        void checkLevelStatus(const float deltaTime);

        // inline ugliness
        inline bool playerLostRound() const
        {
            if(ball.position.y >= windowManager->getHeightf()) {
                return true;
            }
            return false;
        }

        inline bool isGameOver() const
        {
            if(player.lives == 0) {
                return true;
            }
            return false;
        }

        inline void resetPlayerLives()
        {
            player.lives = TotalPlayerLives;
        }

        inline void resetGameLevels()
        {
            // set all bricks back to not destroyed and set current level to 0
            for(std::size_t i{ 0 }; i < bricks.size(); ++i) {
                for(std::size_t j{ 0 }; j < bricks[i].isDestroyed.size(); ++j) {
                    bricks[i].isDestroyed[j] = false;
                }
            }
            currentLevel = 0;
        }

        void resetBallAndPaddle();

        inline bool isLevelFinished() const
        {
            for(std::size_t i{ 0 }; i < bricks[currentLevel].isDestroyed.size(); ++i) {
                if(!bricks[currentLevel].isSolid[i] && !bricks[currentLevel].isDestroyed[i]) {
                    return false;
                }
            }
            return true;
        }

        inline bool isLastLevel() const
        {
            return currentLevel == bricks.size() - 1;
        }

        inline void goNextLevel()
        {
            ++currentLevel;
        }

        void resetLevelEmergency();

        std::array<Bricks, 4> bricks;
        PowerUps powerUps;
        Player player;
        Ball ball;
        Background background;

        Window::WindowManager* windowManager;
        Input::InputManager* inputManager;
        Event::EventManager* eventManager;
        Menu* menu;
        Audio::AudioManager* audioManager;
        Resource::TextureManager* textureManager;
        Renderer::SpriteRenderer* spriteRenderer;
        Effect::ParticleEmitter* particleEmitter;
        Effect::PostProcessor* postProcessor;
        Renderer::TextRenderer* textRenderer;
        Renderer::UIRenderer* uiRenderer;
        Resource::ShaderManager *shaderManager;

        int sceneWidth;
        int sceneHeight;
        float sceneWidthf;
        float sceneHeightf;
        GameState gameState;
        std::size_t currentLevel;
        bool quit;
    };

};
