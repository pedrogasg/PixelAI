
#include "pai_app_base.hpp"

#include "controls/keyboard_movement_controller.hpp"

#include "systems/simple_render_system.hpp"
#include "systems/pixel_system.hpp"

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
        glm::vec2 state = {0.f,0.f};
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

        // SimpleRenderSystem simpleRenderSystem{
        //     paiDevice,
        //     paiRenderer.getSwapChainRenderPass(),
        //     globalSetLayout->getDescriptorSetLayout()};

        PixelSystem pixelSystem{
            paiDevice,
            paiRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};

        PaiCamera camera{};

        auto viewerObject = PaiGameObject::createGameObject();
        viewerObject.transform.translation.z = -5.0f;
        // viewerObject.transform.translation.x = -0.2f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!paiWindow.shouldClose())
        {
            glfwWaitEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            //cameraController.moveInPlaneXZ(paiWindow.getGLFWwindow(), frameTime, viewerObject);
            state = cameraController.moveInPlaneXY(paiWindow.getGLFWwindow(), state);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = paiRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(120.f), aspect, 0.01f, 25.f);

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
                ubo.agent = state;
                //ubo.projection = camera.getProjection();
                //ubo.view = camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                paiRenderer.beginSwapChainRenderPass(commandBuffer);
                //simpleRenderSystem.render(frameInfo);
                pixelSystem.render(frameInfo);

                paiRenderer.endSwapChainRenderPass(commandBuffer);
                paiRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(paiDevice.device());
    }

    std::unique_ptr<PaiModel> createSquareModel(PaiDevice &device, glm::vec3 offset)
    {
        PaiModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            // tail face (green)
            {{-.5f, -.5f, 0.f}, {.1f, .8f, .1f}},
            {{.5f, .5f, 0.f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, 0.f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, 0.f}, {.1f, .8f, .1f}},
        };
        for (auto &v : modelBuilder.vertices)
        {
            v.color += offset;
        }

        modelBuilder.indices = {0, 1, 2, 0, 3, 1};

        return std::make_unique<PaiModel>(device, modelBuilder);
    }

    void PaiAppBase::loadGameObjects()
    {
        // std::shared_ptr<PaiModel> lveModel = createSquareModel(paiDevice, {.0f, .0f, .0f});
        // auto cube = PaiGameObject::createGameObject();
        // cube.model = lveModel;
        // cube.transform.translation = {.0f, .0f, .0f};
        // // cube.transform.scale = {.5f, .5f, .5f};
        // gameObjects.emplace(cube.getId(), std::move(cube));

        auto pixel = std::make_shared<PaiPixel>(paiDevice, 100, 100);
        auto grid = PaiGameObject::createGameObject();
        grid.pixel = pixel;

        gameObjects.emplace(grid.getId(), std::move(grid));
    }
} // namespace pai
