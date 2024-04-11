#include "Breakout.hpp"

#include "KeyCodes.hpp"
#include "Timer.hpp"
#include "Physics.hpp"

#include <glm/gtc/matrix_transform.hpp>

// all of this crap is only to load levels, probably need to store this
// in another class, meh
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

namespace Breakout {
    using namespace Window;
    using namespace Input;
    using namespace Event;
    using namespace Audio;
    using namespace Resource;
    using namespace Renderer;
    using namespace Effect;
    using namespace Physics;

    // ----------------------------
    // player shit
    // ----------------------------
    static constexpr glm::vec2 PlayerSize{ glm::vec2(100.f, 20.f) };
    static constexpr glm::vec2 PlayerMaxVelocity{ glm::vec2(500.f) };

    // ----------------------------
    // ball crap
    // ----------------------------
    static constexpr float BallRadius{ 18.5f };
    static constexpr glm::vec2 InitialBallVelocity{ glm::vec2(200.f, -450.f) };

    // ----------------------------
    // power-ups crap
    // ----------------------------
    static constexpr float PowerUpDurationOnPlayer{ 10.f };
    static constexpr float PowerUpSpeedStrength{ 1.4f };
    static constexpr glm::vec3 PowerUpStickyPaddleColour{ glm::vec3(1.f, .5f, 1.f) };
    static constexpr glm::vec3 PowerUpPassThroughBallColour{ glm::vec3(1.f, 0.f, 0.f) };
    static constexpr float PowerUpPadSizeIncreaseX { 50.f };
    static constexpr glm::vec2 PowerUpVelocity{ glm::vec2(200.f) };

    // -------------------------
    // globals nightmare
    // -------------------------
    static float gShakeTime{ 0.f }; // controls lifetime of shake effect
    static unsigned int gCurrentPowerUpIndex{ 0 }; // bleh

    Game::Game(WindowManager* aWindowManager,
               InputManager* aInputManager,
               EventManager* aEventManager,
               Menu* aMenu,
               AudioManager* aAudioManager,
               TextureManager* aTextureManager,
               SpriteRenderer* aSpriteRenderer,
               ParticleEmitter* aParticleEmitter,
               PostProcessor* aPostProcessor,
               TextRenderer* aTextRenderer,
               UIRenderer* aUIRenderer,
               ShaderManager *aShaderManager)
        :windowManager{ aWindowManager },
         inputManager{ aInputManager },
         eventManager{ aEventManager},
         menu{ aMenu },
         audioManager{ aAudioManager },
         textureManager{ aTextureManager },
         spriteRenderer{ aSpriteRenderer },
         particleEmitter{ aParticleEmitter },
         postProcessor{ aPostProcessor },
         textRenderer{ aTextRenderer },
         uiRenderer{ aUIRenderer },
         shaderManager{ aShaderManager },
         sceneWidth{ windowManager->getWidth()},
         // disgusting hack, this is for the freaking bottom ui
         sceneHeight{ windowManager->getHeight() - 63 },
         sceneWidthf { windowManager->getWidthf() },
         sceneHeightf { windowManager->getHeight() - 63.f },
         gameState{ GameState::MENU },
         currentLevel{ 0 },
         quit{ false }
    {
        // these two events will be posted by the menu
        eventManager->subscribe(EventType::MenuPlayGame, [this]([[maybe_unused]]const Event::Event& event) {
            playGame();
        });

        eventManager->subscribe(EventType::MenuQuitGame, [this]([[maybe_unused]]const Event::Event& event) {
            quitGame();
        });

        initializePlayer();
        initializeBall();
        initializeBackground();
        initializeLevels();
        initializePowerUps();

        audioManager->playMusic(AudioID_bgMusic);
    }

    void Game::run()
    {
        float lastFrame{ 0.f };

        while(!quit) {
            float currentFrame{ Timer::getTime() };
            float deltaTime{ currentFrame - lastFrame };
            lastFrame = currentFrame;

            processInput();
            update(deltaTime);
            render();

            windowManager->swapBuffers();
            windowManager->pollWindowEvents();
        }

        windowManager->closeWindow();
    }

    void Game::processInput()
    {
        inputManager->beginFrame(windowManager);

        // quick way to quit the game
        if(inputManager->isKeyPressed(KeyCodes::KEY_Q)) {
            quit = true;
        }

        if(gameState == GameState::MENU) {
            menu->processInput();
        } else if(gameState == GameState::ACTIVE) {
            if(inputManager->isKeyPressed(KeyCodes::KEY_ESCAPE)) {
                gameState = GameState::MENU;
            } else if(inputManager->isKeyPressed(KeyCodes::KEY_A) ||
                      inputManager->isKeyPressed(KeyCodes::KEY_LEFT)) {
                player.isMovingLeft = true;
            } else if(inputManager->isKeyPressed(KeyCodes::KEY_D) ||
                      inputManager->isKeyPressed(KeyCodes::KEY_RIGHT)) {
                player.isMovingRight = true;
            } else if(inputManager->isKeyPressed(KeyCodes::KEY_SPACE)) {
                ball.isStuck = false;
            } else if(inputManager->isKeyPressed(KeyCodes::KEY_R)) {
                // crappy game has bugs so reset level in case something unfair happens
                resetLevelEmergency();
            }

            if(inputManager->isKeyReleased(KeyCodes::KEY_A) ||
               inputManager->isKeyReleased(KeyCodes::KEY_LEFT)) {
                player.isMovingLeft = false;
            }

            if(inputManager->isKeyReleased(KeyCodes::KEY_D) ||
               inputManager->isKeyReleased(KeyCodes::KEY_RIGHT)) {
                player.isMovingRight = false;
            }
        } else if(gameState == GameState::LOSE) {
            if(inputManager->isKeyPressed(KeyCodes::KEY_ENTER)) {
                gameState = GameState::ACTIVE;
            }
        } else if(gameState == GameState::WIN) {
            if(inputManager->isKeyPressed(KeyCodes::KEY_ENTER)) {
                gameState = GameState::ACTIVE;
            }
        }
    }

    void Game::update(const float deltaTime)
    {
        if(gameState == GameState::ACTIVE) {
            // TODO in next versions of the engine this crap won't be here anymore, rn its not
            // clear where all of these should be, need to investigate further
            updatePlayer(deltaTime);
            updateBall(deltaTime);
            updateCollisions();
            updatePowerUps(deltaTime);

            particleEmitter->updateParticles(deltaTime,
                                             ball.position,
                                             ball.velocity,
                                             glm::vec2(ball.radius * .5f));

            checkLevelStatus(deltaTime);
        }
    }

    void Game::render() const
    {
        if(gameState == GameState::MENU) {
            menu->render();
        } else if(gameState == GameState::ACTIVE) {
            postProcessor->beginRender();
            // TODO cant be bothered now, but in next versions all of this def needs to get fixed
            spriteRenderer->render(player,
                                   ball,
                                   background,
                                   bricks[currentLevel],
                                   powerUps);
            particleEmitter->render();
            postProcessor->endRender();
            postProcessor->render(Timer::getTime());

            uiRenderer->render(player.lives, currentLevel);
        } else if(gameState == GameState::WIN) {
            // draw win
            spriteRenderer->render(shaderManager->getShader("MenuShader"),
                                   textureManager->getTexture("WinBackground"));
        } else if(gameState == GameState::LOSE) {
            // draw lose
            spriteRenderer->render(shaderManager->getShader("MenuShader"),
                                   textureManager->getTexture("LoseBackground"));
        }
    }

    void Game::playGame()
    {
        gameState = GameState::ACTIVE;
    }

    void Game::quitGame()
    {
        quit = true;
    }

    void Game::initializePlayer()
    {
        const float playerX{ sceneWidthf * .5f - (PlayerSize.x * .5f)};
        const float playerY{ sceneHeightf - PlayerSize.y };

        player = Player{
            .model = glm::mat4(1.f),
            .colour = glm::vec3(1.f),
            .position = glm::vec2(playerX, playerY),
            .size = PlayerSize,
            .velocity = glm::vec2(500.f),
            .sprite = {.id = textureManager->getTexture("PaddleTexture")->id},
            .acceleration = 2.f,
            .deceleration = std::pow(2.f, 12),
            .lives = TotalPlayerLives,
            .isMovingLeft = false,
            .isMovingRight = false
        };
    }

    void Game::updatePlayer(const float deltaTime)
    {
        if(player.isMovingLeft) {
            if(player.position.x >= 0.f) {
                player.velocity.x = -PlayerMaxVelocity.x;
                player.position.x += player.velocity.x * deltaTime;
                if(ball.isStuck) {
                    ball.position.x += player.velocity.x * deltaTime;
                }
            }
        } else if(player.isMovingRight) {
            if(player.position.x <= sceneWidthf - player.size.x) {
                player.velocity.x = PlayerMaxVelocity.x;
                player.position.x += player.velocity.x * deltaTime;
                if(ball.isStuck) {
                    ball.position.x += player.velocity.x * deltaTime;
                }
            }
        } else {
            // if the paddle is still moving while not pressing any key, decelerate.
            if(player.velocity.x < 0.f) { // Going left.
                player.velocity.x = std::min(player.velocity.x + player.deceleration * deltaTime, 0.f);
            } else if(player.velocity.x > 0.f) { // Going right.
                player.velocity.x = std::max(player.velocity.x - player.deceleration * deltaTime, 0.f);
            }

            player.position.x += player.velocity.x * deltaTime;

            // NOTE: if the player is very close to the left end and just deccelerated, need to check if it's going
            // out of bounds. In that case, clamp. same for the right end of the screen.
            if(player.position.x <= 0.f) {
                player.position.x = 0.f;
            } else if(player.position.x >= sceneWidthf - player.size.x) {
                player.position.x = sceneWidthf - player.size.x;
            } else {
                if(ball.isStuck) {
                    ball.position.x += player.velocity.x * deltaTime;
                }
            }
        }

        player.model = glm::mat4(1.f);

        player.model = glm::translate(player.model,
                                      glm::vec3(player.position.x, player.position.y, 0.f));

        player.model = glm::translate(player.model,
                                      glm::vec3(.5f * player.size.x, .5f * player.size.y, 0.f));

        player.model = glm::translate(player.model,
                                      glm::vec3(-0.5f * player.size.x, -0.5f * player.size.y, 0.f));

        player.model = glm::scale(player.model,
                                  glm::vec3(player.size, 1.f));
    }

    void Game::initializeBall()
    {
        const glm::vec2 ballPosition{ player.position + glm::vec2((player.size.x / 2.f) - BallRadius,
                                                                  -2.f * BallRadius) };

        const glm::vec2 ballCenter{ glm::vec2(ball.position.x + ball.radius,
                                              ball.position.y + ball.radius) };

        ball = Ball{
            .model  = glm::mat4(1.f),
            .colour = glm::vec3(1.f),
            .position = ballPosition,
            .size = glm::vec2(BallRadius * 2.f, BallRadius * 2.f),
            .velocity = InitialBallVelocity,
            .center = ballCenter,
            .radius = BallRadius,
            .rotation = 0.f,
            .sprite = {.id = textureManager->getTexture("BallTexture")->id },
            .isStuck = true,
            .isSticky = false,
            .isPassingThrough = false
        };
    }

    void Game::updateBall(const float deltaTime)
    {
        if(!ball.isStuck) {
            ball.position += ball.velocity * deltaTime;
        }

        // ZzZZZzzzZzzZZZZZZZZZZZ
        ball.model = glm::mat4(1.f);

        ball.model = glm::translate(ball.model,
                                    glm::vec3(ball.position.x, ball.position.y, 0.f));

        ball.model = glm::translate(ball.model,
                                    glm::vec3(.5f * ball.size.x, .5f * ball.size.y, 0.f));

        ball.model = glm::rotate(ball.model,
                                 Timer::getTime(),
                                 glm::vec3(0.f, 0.f, 1.f));

        ball.model = glm::translate(ball.model,
                                    glm::vec3(-0.5f * ball.size.x, -0.5f * ball.size.y, 0.f));

        ball.model = glm::scale(ball.model,
                                glm::vec3(ball.size, 1.f));

        ball.center = glm::vec2(ball.position.x + ball.radius,
                                ball.position.y + ball.radius);
    }

    void Game::initializePowerUps()
    {
        powerUps.sprites.reserve(100);
        powerUps.colours.reserve(100);
        powerUps.positions.reserve(100);
        powerUps.velocities.reserve(100);
        powerUps.sizes.reserve(100);
        powerUps.durations.reserve(100);
        powerUps.types.reserve(100);
        powerUps.isDestroyed.reserve(100);
        powerUps.isActive.reserve(100);
        powerUps.models.reserve(100);

        for(std::size_t i{ 0 }; i < 100; ++i) {
            powerUps.sprites.emplace_back(DrawableTexture{ .id = 0});
            powerUps.colours.emplace_back(glm::vec3(0.f));
            powerUps.positions.emplace_back(glm::vec2(0.f));
            powerUps.velocities.emplace_back(glm::vec2(0.f));
            powerUps.sizes.emplace_back(glm::vec2(0.f));
            powerUps.durations.emplace_back(0.f);
            powerUps.types.emplace_back(PowerUpType::SPEED);
            powerUps.isDestroyed.emplace_back(true);
            powerUps.isActive.emplace_back(false);
            powerUps.models.emplace_back(glm::mat4(1.f));
        }
    }

    void Game::updatePowerUps(const float deltaTime)
    {
        for(unsigned int i{ 0 }; i < powerUps.positions.size(); ++i) {
            powerUps.positions[i].y += powerUps.velocities[i].y * deltaTime;

            // yep, you know how it goes...
            powerUps.models[i] = glm::mat4(1.f);
            powerUps.models[i] = glm::translate(powerUps.models[i],
                                                glm::vec3(powerUps.positions[i].x,
                                                          powerUps.positions[i].y, 0.f));
            powerUps.models[i] = glm::translate(powerUps.models[i],
                                                glm::vec3(.5f * powerUps.sizes[i].x,
                                                          .5f * powerUps.sizes[i].y, 0.f));
            powerUps.models[i] = glm::translate(powerUps.models[i],
                                                glm::vec3(-0.5f * powerUps.sizes[i].x, -0.5f * powerUps.sizes[i].y, 0.f));
            powerUps.models[i] = glm::scale(powerUps.models[i],
                                            glm::vec3(powerUps.sizes[i], 1.f));

            // Disable effects when power-up runs out.
            if(powerUps.isActive[i]) {
                powerUps.durations[i] -= deltaTime;

                if(powerUps.durations[i] <= 0.f) {
                    powerUps.isActive[i] = false;
                    powerUps.isDestroyed[i] = true;

                    // Remove effects from the ball/player.
                    switch(powerUps.types[i]) {
                    case PowerUpType::SPEED:
                        ball.velocity = InitialBallVelocity;
                        break;
                    case PowerUpType::STICKY:
                        ball.isSticky = false;
                        player.colour = glm::vec3(1.f);
                        break;
                    case PowerUpType::PASSTHROUGH:
                        ball.isPassingThrough = false;
                        ball.colour = glm::vec3(1.f);
                        break;
                    case PowerUpType::PADSIZEINCREASE:
                        player.size = PlayerSize;
                        player.colour = glm::vec3(1.f);
                        break;
                    case PowerUpType::CONFUSE:
                        postProcessor->isConfusing = false;
                        break;
                    case PowerUpType::CHAOS:
                        postProcessor->isChaotic = false;
                        break;
                    }
                }
            }
        }
    }

    void Game::initializeBackground()
    {
        background = Background{
            .model  = glm::mat4(1.f),
            .colour = glm::vec3(1.f),
            .position = glm::vec2(0.f),
            .size = glm::vec2(sceneWidthf, sceneHeightf),
            .velocity = glm::vec2(0.f),
            .sprite = {.id = textureManager->getTexture("BackgroundTexture")->id }
        };

        // blehhhhhh, doing this because of the tutorial
        background.model = glm::translate(background.model,
                                          glm::vec3(background.position.x,
                                                    background.position.y,
                                                    0.f)
                                          );

        background.model = glm::translate(background.model,
                                          glm::vec3(.5f * background.size.x,
                                                    .5f * background.size.y,
                                                    0.f)
                                          );

        background.model = glm::translate(background.model,
                                          glm::vec3(-0.5f * background.size.x,
                                                    -0.5f * background.size.y,
                                                    0.f));

        background.model = glm::scale(background.model,
                                      glm::vec3(background.size, 1.f));
    }

    // shit only needed for level loading
#define BLOCK_SOLID 1
#define BLOCK_NON_SOLID_LIGHT_BLUE 2
#define BLOCK_NON_SOLID_GREEN 3
#define BLOCK_NON_SOLID_LIGHT_YELLOW 4
#define BLOCK_NON_SOLID_ORANGE 5

    void Game::initializeLevels()
    {

        static const std::array<std::filesystem::path, 4> levelsFilesPath{
            "./res/levels/one",
            "./res/levels/two",
            "./res/levels/three",
            "./res/levels/four",
        };

        for(std::size_t i{ 0 }; i < bricks.size(); ++i) {
            if(!loadGameLevelFromFile(levelsFilesPath[i], i)) {
                std::cerr << "Couldn't load game level " << levelsFilesPath[i] << '\n';
                exit(EXIT_FAILURE);
            }
        }
    }

    bool Game::loadGameLevelFromFile(const std::filesystem::path& filePath,
                                     const std::size_t levelIndex)
    {
        std::ifstream fileIfs(filePath);
        if(!fileIfs) {
            std::cerr << "Couldn't open game level at path: " << filePath << '\n';
            return false;
        }

        std::string line;
        std::vector<std::vector<unsigned int>> tileData;
        unsigned int tileNumber{ 0 };

        while(std::getline(fileIfs, line)) {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;

            while(sstream >> tileNumber) {
                row.push_back(tileNumber);
            }

            tileData.push_back(row);
        }

        if(tileData.empty()) {
            return false;
        }

        // these two variables store the size of each brick.
        // the idea is to distribute them evenly across the screen.
        const float brickWidth{ sceneWidthf / static_cast<float>(tileData[0].size()) };
        const float brickHeight{ sceneHeightf * 0.5f / static_cast<float>(tileData.size()) };
        const glm::vec2 size(brickWidth, brickHeight);

        // disgusting
        auto solidBlockTexture = textureManager->getTexture("BlockSolidTexture");
        auto blockTexture = textureManager->getTexture("BlockTexture");

        for(unsigned int i{ 0 }; i < tileData.size(); ++i) {
            for(unsigned int j{ 0 }; j < tileData[0].size(); ++j) {
                const glm::vec2 position(brickWidth * static_cast<float>(j),
                                         brickHeight * static_cast<float>(i));
                glm::mat4 model{ glm::mat4(1.f) };
                model = glm::translate(model, glm::vec3(position.x, position.y, 0.f));
                model = glm::translate(model, glm::vec3(.5f * size.x, .5f * size.y, 0.f));
                model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
                model = glm::scale(model, glm::vec3(size, 1.f));

                if(tileData[i][j] == BLOCK_SOLID) {
                    bricks[levelIndex].sprites.emplace_back(DrawableTexture{.id = solidBlockTexture->id});
                    bricks[levelIndex].colours.emplace_back(glm::vec3(.8f, .8f, .7f));
                    bricks[levelIndex].positions.emplace_back(position);
                    bricks[levelIndex].sizes.emplace_back(size);
                    bricks[levelIndex].isSolid.emplace_back(true);
                    bricks[levelIndex].isDestroyed.emplace_back(false);
                    bricks[levelIndex].models.emplace_back(model);
                } else if (tileData[i][j] > BLOCK_SOLID) {
                    glm::vec3 colour{ glm::vec3(1.f) };

                    if(tileData[i][j] == BLOCK_NON_SOLID_LIGHT_BLUE) {
                        colour = glm::vec3(0.2f, 0.6f, 1.f);
                    } else if(tileData[i][j] == BLOCK_NON_SOLID_GREEN) {
                        colour = glm::vec3(0.f, .7f, 0.f);
                    } else if(tileData[i][j] == BLOCK_NON_SOLID_LIGHT_YELLOW) {
                        colour = glm::vec3(.8f, .8f, .4f);
                    } else if(tileData[i][j] == BLOCK_NON_SOLID_ORANGE) {
                        colour = glm::vec3(1.f, .5f, 0.f);
                    }

                    bricks[levelIndex].sprites.emplace_back(DrawableTexture{ .id = blockTexture->id });
                    bricks[levelIndex].colours.emplace_back(colour);
                    bricks[levelIndex].positions.emplace_back(position);
                    bricks[levelIndex].sizes.emplace_back(size);
                    bricks[levelIndex].isSolid.emplace_back(false);
                    bricks[levelIndex].isDestroyed.emplace_back(false);
                    bricks[levelIndex].models.emplace_back(model);
                }
            }
        }

        return true;
    }

    void Game::updateCollisions()
    {
        // ugliest function ever
        static constexpr float Strength{ 2.f };
        CollisionData collisionData;

        //
        // TODO !!! not happy with this at all, in next versions this will be gone !!!
        //

        // window's dimensions <--> ball
        if(ball.position.x <= 0.f) {
            ball.velocity.x = -(ball.velocity.x);
            ball.position.x = 0.f;
        } else if(ball.position.x + ball.size.x >= sceneWidthf) {
            ball.velocity.x = -(ball.velocity.x);
            ball.position.x = sceneWidthf - ball.size.x;
        } else if(ball.position.y <= 0.f) {
            ball.velocity.y = -(ball.velocity.y);
            ball.position.y = 0.f;
        }

        // ball <--> bricks collisions, if there's any, try spawn power-up
        // and adjust ball's velocity vector
        for(unsigned int i{ 0 }; i < bricks[currentLevel].positions.size(); ++i) {
            if(!bricks[currentLevel].isDestroyed[i]) {
                if(Physics::checkSweptCircleAABBCollision(ball.center,
                                                          ball.radius,
                                                          ball.velocity,
                                                          bricks[currentLevel].sizes[i],
                                                          bricks[currentLevel].positions[i],
                                                          collisionData)) {
                    if(!bricks[currentLevel].isSolid[i]) {
                        audioManager->playChunk(AudioID_bhitNonSolidBrick);
                        bricks[currentLevel].isDestroyed[i] = true;
                        spawnPowerUp(bricks[currentLevel].positions[i]);
                    } else {
                        audioManager->playChunk(AudioID_bhitSolidBrick);
                        postProcessor->isShaking = true;
                        gShakeTime = Effect::ShakeTime;
                    }

                    if(!ball.isPassingThrough || bricks[currentLevel].isSolid[i]) {
                        // TODO bad bad bad stuff, will have a better version on next game
                        // react to collision

                        // get the ball out of the brickzzz
                        ball.position.x -= collisionData.normal.x * collisionData.depth;
                        ball.position.y -= collisionData.normal.y * collisionData.depth;

                        // determine where the ball should go, careful with brick corners
                        if((collisionData.normal.x != 0.f) && collisionData.normal.y != 0.f) {
                            // look ma, corner case

                            // came up with this sad stuff
                            const float dotX{ glm::abs(glm::dot(glm::vec2(0.f, 1.f), collisionData.normal)) };
                            const float dotY{ glm::abs(glm::dot(glm::vec2(1.f, 0.f), collisionData.normal)) };

                            if(dotX > dotY) { // change direction, biatch, based on sharpest impact angle
                                // ball.velocity.x = -ball.velocity.x;
                                ball.velocity.y = -ball.velocity.y;
                            } else {
                                ball.velocity.x = -ball.velocity.x;
                                // ball.velocity.y = -ball.velocity.y;
                            }
                        } else if(collisionData.normal.x != 0.f) {
                            ball.velocity.x = -ball.velocity.x;
                        } else if(collisionData.normal.y != 0.f) {
                            ball.velocity.y = -ball.velocity.y;
                        }
                    }
                }
            }
        }

        // ball <--> player's paddle collisions
        if(!ball.isStuck) {
            if(Physics::checkSweptCircleAABBCollision(ball.center,
                                                      ball.radius,
                                                      ball.velocity,
                                                      player.size,
                                                      player.position,
                                                      collisionData)) {
                // get the ball out of the brickzzz x2
                ball.position.x -= collisionData.normal.x * collisionData.depth;
                ball.position.y -= collisionData.normal.y * collisionData.depth;

                // adjust ball's velocity depending on where it hits. as the ball approaches the left or right
                // ends it will gain more horizontal "speed".
                const float centerPaddle{ player.position.x + player.size.x * .5f };
                const float distance{ (ball.position.x + ball.radius) - centerPaddle };
                const float percentage{ distance / (player.size.x / 2.f) };
                glm::vec2 oldVelocity{ ball.velocity };
                ball.velocity.x = InitialBallVelocity.x * percentage * Strength;
                ball.velocity.y = -1.f * std::abs(ball.velocity.y);
                ball.velocity = glm::normalize(ball.velocity) * glm::length(oldVelocity);
                ball.isStuck = ball.isSticky;

                audioManager->playChunk(AudioID_bhitPlayer);
            }
        }

        // power-ups <--> player's paddle collisions
        for(unsigned int i{ 0 }; i < powerUps.positions.size(); ++i) {
            if(!powerUps.isDestroyed[i]) {
                if(Physics::checkAABBCollision(player.position,
                                               player.size,
                                               powerUps.positions[i],
                                               powerUps.sizes[i])) {
                    reActivatePowerUp(i);
                    audioManager->playChunk(AudioID_powerUp);
                }
            }
        }
    }

    void Game::checkLevelStatus(const float deltaTime)
    {
        // shake effect, when ball hits a solid brick, gShakeTime will be set to N and each frame
        // decrease its duration; when it gets to 0, remove effect
        if(gShakeTime > 0.f) {
            gShakeTime -= deltaTime;
            if(gShakeTime <= 0.f) {
                postProcessor->isShaking = false;
            }
        }

        if(playerLostRound()) {
            --player.lives;

            if(isGameOver()) {
                // TODO game over
                gameState = GameState::LOSE;
                resetPlayerLives();
                resetGameLevels();
            }

            removePowerUpEffects();
            resetBallAndPaddle();
        } else if(isLevelFinished()) {

            // if player beats level, check if game is finished
            if(isLastLevel()) {
                audioManager->playChunk(AudioID_winGame);
                gameState = GameState::WIN;
                resetGameLevels();
            } else {
                audioManager->playChunk(AudioID_roundWin);
                goNextLevel();
            }

            removePowerUpEffects();
            resetPlayerLives();
            resetBallAndPaddle();
        }
    }

    void Game::resetBallAndPaddle()
    {
        // center player's paddle
        player.position =
            glm::vec2(
                      (sceneWidthf * .5f) - (player.size.x * .5f),
                      sceneHeightf - player.size.y
                      );

        // put ball at the center of player's paddle
        ball.position = player.position + glm::vec2((player.size.x / 2.f) - BallRadius,
                                                    -2.f * BallRadius);

        ball.isStuck = true;
        player.isMovingRight = false;
        player.isMovingLeft = false;
    }

    void Game::spawnPowerUp(const glm::vec2& referencePosition)
    {
        // spawn multiple power-ups at the same time to be annoying.
        // good power-ups have a slower chance to appear. bad ones, higher chance.

        if(powerUpShouldSpawn(75)) { // 1 of 75 chance aka 1.3%
            auto* texture = textureManager->getTexture("PowerUpSpeed");
            addPowerUp(PowerUpType::SPEED,
                       glm::vec3(.5f, .5f, 1.f),
                       referencePosition,
                       PowerUpVelocity,
                       PowerUpDurationOnPlayer,
                       texture->getWidth(),
                       texture->getHeight(),
                       texture);
        }

        if(powerUpShouldSpawn(75)) {
            auto* texture = textureManager->getTexture("PowerUpSticky");
            addPowerUp(PowerUpType::STICKY,
                       glm::vec3(1.f, .5f, 1.f),
                       referencePosition,
                       PowerUpVelocity,
                       PowerUpDurationOnPlayer,
                       texture->getWidth(),
                       texture->getHeight(),
                       texture);
        }

        if(powerUpShouldSpawn(75)) {
            auto* texture = textureManager->getTexture("PowerUpPassThrough");
            addPowerUp(PowerUpType::PASSTHROUGH,
                       glm::vec3(.5f, 1.f, .5f),
                       referencePosition,
                       PowerUpVelocity,
                       PowerUpDurationOnPlayer,
                       texture->getWidth(),
                       texture->getHeight(),
                       texture);
        }

        if(powerUpShouldSpawn(75)) {
            auto* texture = textureManager->getTexture("PowerUpPadSizeIncrease");
            addPowerUp(PowerUpType::PADSIZEINCREASE,
                       glm::vec3(1.f, .6f, .4f),
                       referencePosition,
                       PowerUpVelocity,
                       PowerUpDurationOnPlayer,
                       texture->getWidth(),
                       texture->getHeight(),
                       texture);
        }

        if(powerUpShouldSpawn(25)) {
            auto* texture = textureManager->getTexture("PowerUpConfuse");
            addPowerUp(PowerUpType::CONFUSE,
                       glm::vec3(1.f, .3f, .3f),
                       referencePosition,
                       PowerUpVelocity,
                       PowerUpDurationOnPlayer,
                       texture->getWidth(),
                       texture->getHeight(),
                       texture);
        }

        if(powerUpShouldSpawn(25)) {
            auto* texture = textureManager->getTexture("PowerUpChaos");
            addPowerUp(PowerUpType::CHAOS,
                       glm::vec3(.9f, .25f, .25f),
                       referencePosition,
                       PowerUpVelocity,
                       PowerUpDurationOnPlayer,
                       texture->getWidth(),
                       texture->getHeight(),
                       texture);
        }
    }

    void Game::reActivatePowerUp(const std::size_t index)
    {
        // scan through power-ups to see if there's another one of the same type that's active.
        // in that case, reset its duration.
        bool reActivated{ false };

        for(unsigned int i{ 0 }; i < powerUps.types.size(); ++i) {
            if(i != index && powerUps.types[i] == powerUps.types[index] &&
               powerUps.isActive[i] && powerUps.durations[i] >= 0.f) {
                // found a similar powerUp, reset duration and quit
                powerUps.durations[i] = PowerUpDurationOnPlayer;
                reActivated = true;
                break;
            }
        }

        // if the player didn't have any other similar power-up active, activate a new one
        if(!reActivated) {
            switch(powerUps.types[index]) {
            case PowerUpType::SPEED:
                ball.velocity *= PowerUpSpeedStrength;
                break;
            case PowerUpType::STICKY:
                ball.isSticky = true;
                player.colour = PowerUpStickyPaddleColour;
                break;
            case PowerUpType::PASSTHROUGH:
                ball.isPassingThrough = true;
                ball.colour = PowerUpPassThroughBallColour;
                break;
            case PowerUpType::PADSIZEINCREASE:
                player.size.x += PowerUpPadSizeIncreaseX;
                break;
            case PowerUpType::CONFUSE:
                postProcessor->isConfusing = true;
                break;
            case PowerUpType::CHAOS:
                postProcessor->isChaotic = true;
                break;
            }
        }

        powerUps.isActive[index] = true;
        powerUps.isDestroyed[index] = true;
    }

    void Game::removePowerUpEffects()
    {
        // remove power-ups from player and ball, and clear.
        for(unsigned int i{ 0 }; i < powerUps.types.size(); ++i) {
            switch(powerUps.types[i]) {
            case PowerUpType::SPEED:
                ball.velocity = InitialBallVelocity;
                break;
            case PowerUpType::STICKY:
                ball.isSticky = false;
                player.colour = glm::vec3(1.f);
                break;
            case PowerUpType::PASSTHROUGH:
                ball.isPassingThrough = false;
                ball.colour = glm::vec3(1.f);
                break;
            case PowerUpType::PADSIZEINCREASE:
                player.size = PlayerSize;
                player.colour = glm::vec3(1.f);
                break;
            case PowerUpType::CONFUSE:
                postProcessor->isConfusing = false;
                break;
            case PowerUpType::CHAOS:
                postProcessor->isChaotic = false;
                break;
            }
        }

        clearPowerUps();
    }

    void Game::clearPowerUps()
    {
        for(unsigned int i{ 0 }; i < powerUps.sprites.size(); ++i) {
            powerUps.sprites[i].id = 0;
            powerUps.colours[i] = glm::vec3(1.f);
            powerUps.positions[i] = glm::vec3(1.f);
            powerUps.velocities[i] = glm::vec3(0.f);
            powerUps.sizes[i] = glm::vec2(0.f);
            powerUps.models[i] = glm::mat4(1.f);
            powerUps.durations[i] = 0.f;
            powerUps.types[i] = PowerUpType::SPEED;
            powerUps.isDestroyed[i] = false;
            powerUps.isActive[i] = false;
        }
    }

    void Game::addPowerUp(const PowerUpType type,
                          const glm::vec3& colour,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          const float duration,
                          const int width,
                          const int height,
                          const Texture* texture)
    {
        const auto nextPowerUpIndex = getNextPowerUpIndex();

        powerUps.sprites[nextPowerUpIndex].id = texture->id;
        powerUps.colours[nextPowerUpIndex] = colour;
        powerUps.positions[nextPowerUpIndex] = position;
        powerUps.velocities[nextPowerUpIndex] = velocity;
        powerUps.sizes[nextPowerUpIndex] = glm::vec2(width * 0.2f, height * 0.2f);
        powerUps.durations[nextPowerUpIndex] = duration;
        powerUps.types[nextPowerUpIndex] = type;
        powerUps.isDestroyed[nextPowerUpIndex] = false;
        powerUps.isActive[nextPowerUpIndex] = false;
    }

    std::size_t Game::getNextPowerUpIndex() const
    {
        for(std::size_t i{ gCurrentPowerUpIndex }; i < powerUps.positions.size(); ++i) {
            if(powerUps.isDestroyed[i] && !powerUps.isActive[i] && powerUps.durations[i] <= 0.f) {
                gCurrentPowerUpIndex = i;
                return gCurrentPowerUpIndex;
            }
        }

        for(std::size_t i{ 0 }; i < gCurrentPowerUpIndex; ++i) {
            if(powerUps.isDestroyed[i] && !powerUps.isActive[i] && powerUps.durations[i] <= 0.f) {
                gCurrentPowerUpIndex = i;
                return gCurrentPowerUpIndex;
            }
        }

        return gCurrentPowerUpIndex++;
    }

    void Game::resetLevelEmergency()
    {
        removePowerUpEffects();
        resetBallAndPaddle();
    }
};
