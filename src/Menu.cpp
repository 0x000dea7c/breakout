#include "Menu.hpp"

#include "KeyCodes.hpp"

#include <glm/glm.hpp>

#include <sstream>

namespace Breakout {

    static constexpr glm::vec3 MenuOptionColour{ glm::vec3(.25f, .29f, .4f) };
    static constexpr glm::vec3 HighlightedColour{ glm::vec3(.0f, .0f, 0.f) };

    using namespace Event;
    using namespace Input;
    using namespace Resource;
    using namespace Renderer;
    using namespace Window;
    using namespace Audio;

    Menu::Menu(EventManager* aEventManager,
               InputManager* aInputManager,
               const Shader* aShader,
               const Texture* aTexture,
               TextRenderer* aTextRenderer,
               WindowManager* aWindowManager,
               AudioManager* aAudioManager)
        :eventManager{ aEventManager },
         inputManager{ aInputManager },
         shader{ aShader },
         texture{ aTexture },
         textRenderer{ aTextRenderer },
         windowManager{ aWindowManager },
         audioManager{ aAudioManager },
         currentItem{ MenuItem::PLAY },
         VAO{ 0 }
    {
        // init vertex data
        unsigned int VBO{ 0 };
        constexpr std::array<float, 24> vertices{
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
    }

    Menu::~Menu()
    {
        glDeleteVertexArrays(1, &VAO);
    }

    void Menu::processInput()
    {
        if(inputManager->isKeyPressed(KeyCodes::KEY_W) ||
           inputManager->isKeyPressed(KeyCodes::KEY_UP)) {
            --currentItem;
        } else if(inputManager->isKeyPressed(KeyCodes::KEY_S) ||
                  inputManager->isKeyPressed(KeyCodes::KEY_DOWN)) {
            ++currentItem;
        } else if(inputManager->isKeyPressed(KeyCodes::KEY_ESCAPE)) {
            eventManager->post(Event::Event(EventType::MenuPlayGame));
        } else if(currentItem == MenuItem::SOUND &&
                  (inputManager->isKeyPressed(KeyCodes::KEY_LEFT) ||
                   inputManager->isKeyPressed(KeyCodes::KEY_A))) {
            audioManager->decreaseVolume();
        } else if(currentItem == MenuItem::SOUND &&
                  (inputManager->isKeyPressed(KeyCodes::KEY_RIGHT) ||
                   inputManager->isKeyPressed(KeyCodes::KEY_D))) {
            audioManager->increaseVolume();
        } else if(currentItem == MenuItem::PLAY &&
                  inputManager->isKeyPressed(KeyCodes::KEY_ENTER)) {
            eventManager->post(Event::Event(EventType::MenuPlayGame));
        } else if(currentItem == MenuItem::QUIT &&
                  inputManager->isKeyPressed(KeyCodes::KEY_ENTER)) {
            eventManager->post(Event::Event(EventType::MenuQuitGame));
        }
    }

    // TODO bluh blah bloh, in next versions the menu won't render anything,
    // that'd be the renderer's task!
    void Menu::render() const
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glActiveTexture(GL_TEXTURE0);
        texture->bind();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // cant be fucking bothered now
        textRenderer->renderText("Play",
                                 (windowManager->getWidthf() * .5f) - 35.f,
                                 (windowManager->getHeightf() * .5f),
                                 1.f,
                                 (currentItem == MenuItem::PLAY) ? HighlightedColour : MenuOptionColour);

        textRenderer->renderText("Sound:",
                                 (windowManager->getWidthf() * .5f) - 35.f,
                                 (windowManager->getHeightf() * .5f) + 40.f,
                                 1.f,
                                 (currentItem == MenuItem::SOUND) ? HighlightedColour : MenuOptionColour);

        std::stringstream ss;
        ss << "<" << audioManager->getNormalizedVolume() << ">";
        textRenderer->renderText(ss.str(),
                                 (windowManager->getWidthf() * .5f) + 65.f,
                                 (windowManager->getHeightf() * .5f) + 40.f,
                                 1.f,
                                 (currentItem == MenuItem::SOUND) ? HighlightedColour : MenuOptionColour);

        textRenderer->renderText("Quit",
                                 (windowManager->getWidthf() * .5f) - 35.f,
                                 (windowManager->getHeightf() * .5f) + 80.f,
                                 1.f,
                                 (currentItem == MenuItem::QUIT) ? HighlightedColour : MenuOptionColour);
    }
};
