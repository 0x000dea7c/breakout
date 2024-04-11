#include "Window.hpp"

#include <iostream>

namespace Window {

    static void framebufferSizeCallback([[maybe_unused]]GLFWwindow* window,
                                        int width,
                                        int height)
    {
        glViewport(0, 0, width, height);
    }

    static void mouseCallback([[maybe_unused]]GLFWwindow* window,
                              [[maybe_unused]]double xposIn,
                              [[maybe_unused]]double yposIn)
    {

    }

    static void scrollCallback([[maybe_unused]]GLFWwindow* window,
                               [[maybe_unused]]double xoffset,
                               [[maybe_unused]]double yoffset)
    {

    }

    static void GLAPIENTRY messageCallback([[maybe_unused]]GLenum source,
                                           GLenum type,
                                           [[maybe_unused]]GLuint id,
                                           GLenum severity,
                                           [[maybe_unused]]GLsizei length,
                                           const GLchar* message,
                                           [[maybe_unused]]const void* userParam) {
        std::cerr << "GL CALLBACK: "
                  << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
                  << " type = 0x" << std::hex << type
                  << ", severity = 0x" << severity
                  << ", message = " << message << std::endl;
    }

    WindowManager::WindowManager()
        :width{ 1368 },
         height{ 768 },
         widthf{ 1368.f },
         heightf{ 768.f }
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, "Breakout", nullptr, nullptr);

        if(!window) {
            std::cerr << "Couldn't create glfw window.\n";
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetScrollCallback(window, scrollCallback);

        // glad shit
        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cerr << "Couldn't initialize GLAD\n";
            exit(EXIT_FAILURE);
        }

        // Enable VSYNC.
        glfwSwapInterval(1);

        // yep, yep, need to have functions to alter OpenGL capabilities, that's on
        // the TODO list
        glViewport(0, 0, width, height);
        glEnable(GL_BLEND);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(messageCallback, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    WindowManager::~WindowManager()
    {
        glfwDestroyWindow(window);
    }

};
