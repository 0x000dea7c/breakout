#include "pch.hpp"

#include "SubSystems.hpp"
#include "Window.hpp"
#include "Breakout.hpp"
#include "Event.hpp"
#include "Menu.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextRenderer.hpp"
#include "Audio.hpp"
#include "SpriteRenderer.hpp"
#include "ParticleEmitter.hpp"
#include "PostProcessor.hpp"

#include <cstdlib>

int main()
{
    // if any of these mfs fail, the game will close with an err msg
    SubSystems::SubSystems s;

    Window::WindowManager windowManager;
    Input::InputManager inputManager;
    Event::EventManager eventManager;

    // loads game's textures on its constructor
    Resource::TextureManager textureManager;

    // loads and compiles game's shaders on its constructor
    Resource::ShaderManager shaderManager(windowManager.getWidthf(),
                                          windowManager.getHeightf());

    Renderer::TextRenderer textRenderer("./res/fonts/FreeMonoBold.ttf",
                                        28,
                                        shaderManager.getShader("TextRendererShader"));

    Audio::AudioManager audioManager;

    Renderer::SpriteRenderer spriteRenderer(shaderManager.getShader("SpriteShader"));

    Effect::ParticleEmitter particleEmitter(shaderManager.getShader("ParticleShader"),
                                            textureManager.getTexture("ParticleTexture"));

    Effect::PostProcessor postProcessor(windowManager.getWidth(),
                                        windowManager.getHeight(),
                                        shaderManager.getShader("PostProcessorShader"));

    Renderer::UIRenderer uiRenderer(&windowManager,
                                    shaderManager.getShader("UIRendererShader"),
                                    &textureManager,
                                    &textRenderer);

    Breakout::Menu menu(&eventManager,
                        &inputManager,
                        shaderManager.getShader("MenuShader"),
                        textureManager.getTexture("MenuTexture"),
                        &textRenderer,
                        &windowManager,
                        &audioManager);

    Breakout::Game game(&windowManager,
                        &inputManager,
                        &eventManager,
                        &menu,
                        &audioManager,
                        &textureManager,
                        &spriteRenderer,
                        &particleEmitter,
                        &postProcessor,
                        &textRenderer,
                        &uiRenderer,
                        &shaderManager);

    game.run();

    return EXIT_SUCCESS;
}
