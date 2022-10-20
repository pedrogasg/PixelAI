#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace pai
{
    class PaiWindow
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
        PaiWindow(int w, int h, std::string name);
        ~PaiWindow();

        PaiWindow(const PaiWindow &) = delete;
        PaiWindow &operator=(const PaiWindow &) = delete;

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