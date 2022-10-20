#include "pai_window.hpp"

#include <stdexcept>

namespace pai
{
    PaiWindow::PaiWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    PaiWindow::~PaiWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void PaiWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void PaiWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto paiWindow = reinterpret_cast<PaiWindow *>(glfwGetWindowUserPointer(window));
        paiWindow->framebufferResized = true;
        paiWindow->width = width;
        paiWindow->height = height;
    }

    void PaiWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }
}