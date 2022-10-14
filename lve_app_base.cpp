#include "lve_app_base.hpp"
#include "keyboard_movement_controller.hpp"
#include "simple_render_system.hpp"
#include "lve_camera.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <memory>
#include <chrono>

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
        LveCamera camera{};

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!lveWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

            if (auto commandBuffer = lveRenderer.beginFrame())
            {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);

                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void LveAppBase::loadGameObjects()
    {
        std::shared_ptr<LveModel> lveModel =
            LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
        auto gameObj = LveGameObject::createGameObject();
        gameObj.model = lveModel;
        gameObj.transform.translation = {.0f, .0f, 2.5f};
        gameObj.transform.scale = glm::vec3(3.f);
        gameObjects.push_back(std::move(gameObj));
    }
}