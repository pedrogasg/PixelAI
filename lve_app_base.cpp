#include "lve_app_base.hpp"

#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

namespace lve
{
    struct SimplePushConstantData
    {
        glm::mat2 transform{1.f};
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    LveAppBase::LveAppBase()
    {
        loadGameObjects();
    }

    LveAppBase::~LveAppBase() {}

    void LveAppBase::run()
    {
        SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
        while (!lveWindow.shouldClose())
        {
            glfwPollEvents();
            if (auto commandBuffer = lveRenderer.beginFrame())
            {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(lveDevice.device());
    }

    void LveAppBase::sierpinski(
        std::vector<LveModel::Vertex> &vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top)
    {
        if (depth <= 0)
        {
            vertices.push_back({top, {1.0f, 0.0f, 0.0f}});
            vertices.push_back({right, {0.0f, 1.0f, 0.0f}});
            vertices.push_back({left, {0.0f, 0.0f, 1.0f}});
        }
        else
        {
            auto leftTop = 0.5f * (left + top);
            auto rightTop = 0.5f * (right + top);
            auto leftRight = 0.5f * (left + right);
            sierpinski(vertices, depth - 1, left, leftRight, leftTop);
            sierpinski(vertices, depth - 1, leftRight, right, rightTop);
            sierpinski(vertices, depth - 1, leftTop, rightTop, top);
        }
    }

    // void LveAppBase::loadGameObjects()
    // {
    //     std::vector<LveModel::Vertex> vertices{
    //         {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    //         {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    //         {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    //     auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

    //     auto triangle = LveGameObject::createGameObject();
    //     triangle.model = lveModel;
    //     triangle.color = {.1f, .8f, .1f};
    //     triangle.transform2d.translation.x = .2f;
    //     triangle.transform2d.scale = {2.f, .5f};
    //     triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    //     gameObjects.push_back(std::move(triangle));
    // }
    void LveAppBase::loadGameObjects()
    {
        std::vector<LveModel::Vertex> vertices{};
        sierpinski(vertices, 1, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
        auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

        std::vector<glm::vec3> colors{
            {1.f, .7f, .73f},
            {1.f, .87f, .73f},
            {1.f, 1.f, .73f},
            {.73f, 1.f, .8f},
            {.73, .88f, 1.f} //
        };

        for (auto &color : colors)
        {
            color = glm::pow(color, glm::vec3{2.2f});
        }
        for (int i = 0; i < 40; i++)
        {
            auto triangle = LveGameObject::createGameObject();
            triangle.model = lveModel;
            triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
            triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
            triangle.color = colors[i % colors.size()];
            gameObjects.push_back(std::move(triangle));
        }
    }
}