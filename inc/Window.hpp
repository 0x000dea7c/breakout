#pragma once

#include "pch.hpp"

#include <GLFW/glfw3.h>

// the plan is to get rid of glfw3 dependency and code my own to have
// a better understanding of what's going on, but for the first game
// this suffices
namespace Window {

    class WindowManager final {
    public:
        WindowManager();
        ~WindowManager();

        // yep yep
        inline auto getWidth() const { return width; }
        inline auto getHeight() const { return height; }
        inline auto getWidthf() const { return widthf; }
        inline auto getHeightf() const { return heightf; }

        inline GLFWwindow* getWindow() const { return window; } // TODO bleeeeeeeeeeeeeeeeh
        inline void closeWindow() const { glfwSetWindowShouldClose(window, true); }
        inline void swapBuffers() const { glfwSwapBuffers(window); }
        inline void pollWindowEvents() const { glfwPollEvents(); }

    private:
        GLFWwindow* window;

        // yep
        int width;
        int height;
        float widthf;
        float heightf;
    };

};
