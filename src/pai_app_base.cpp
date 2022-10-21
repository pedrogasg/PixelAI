
#include "pai_app_base.hpp"

#include "controls/keyboard_movement_controller.hpp"


#include "systems/unit_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace pai
{

    PaiAppBase::PaiAppBase()
    {
        globalPool =
            PaiDescriptorPool::Builder(paiDevice)
                .setMaxSets(PaiSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, PaiSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
        loadGameObjects();
    }

    PaiAppBase::~PaiAppBase() {}

    void PaiAppBase::run()
    {
        std::vector<std::unique_ptr<PaiBuffer>> uboBuffers(PaiSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<PaiBuffer>(
                paiDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout =
            PaiDescriptorSetLayout::Builder(paiDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(PaiSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            PaiDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        UnitSystem unitSystem{
            paiDevice,
            paiRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};

        PaiCamera camera{};

        auto viewerObject = PaiGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!paiWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(paiWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = paiRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            if (auto commandBuffer = paiRenderer.beginFrame())
            {
                int frameIndex = paiRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects};

                // update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                unitSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                paiRenderer.beginSwapChainRenderPass(commandBuffer);

                unitSystem.render(frameInfo);

                paiRenderer.endSwapChainRenderPass(commandBuffer);
                paiRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(paiDevice.device());
    }

    void PaiAppBase::loadGameObjects()
    {
        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f} //
        };

        for (int i = 0; i < lightColors.size(); i++)
        {
            for (int j = 0; j < lightColors.size()*2; j++)
            {
                auto unitPoint = PaiGameObject::makeUnitPoint(0.4f);
                unitPoint.color = lightColors[i];
                //unitPoint.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
                unitPoint.transform.translation = {(j*.2f) - 1.4f, -i*.2f, 0.f};
                gameObjects.emplace(unitPoint.getId(), std::move(unitPoint));
            }
        }
    }

} // namespace pai
