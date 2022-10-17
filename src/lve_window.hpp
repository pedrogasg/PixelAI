#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace lve
{
    class LveWindow
    {
    private:
        int width;
        int height;
        bool framebufferResized = false;
        std::string windowName;
        GLFWwindow *window;

        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();

    public:
        LveWindow(int w, int h, std::string name);
        ~LveWindow();

        LveWindow(const LveWindow &) = delete;
        LveWindow &operator=(const LveWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }
        GLFWwindow *getGLFWwindow() const { return window; }
        int getWidth() { return width; }
        int getHeight() { return height; }
    };
}