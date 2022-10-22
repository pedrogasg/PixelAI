
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
        glm::vec2 state = {0.f, 0.f};
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
            // glfwPollEvents();
            glfwWaitEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            // cameraController.moveInPlaneXZ(paiWindow.getGLFWwindow(), frameTime, viewerObject);
            glm::vec4 actions = gameObjects.begin()->second.pixel->getActions(state);
            state = cameraController.moveInPlaneXY(paiWindow.getGLFWwindow(), state, actions);
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
                // ubo.projection = camera.getProjection();
                // ubo.view = camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                paiRenderer.beginSwapChainRenderPass(commandBuffer);
                // simpleRenderSystem.render(frameInfo);
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

        std::vector<glm::vec2> walls = {{0, 8},
                                        {0, 9},
                                        {0, 12},
                                        {0, 13},
                                        {0, 17},
                                        {0, 18},
                                        {0, 21},
                                        {0, 22},
                                        {0, 23},
                                        {1, 0},
                                        {1, 4},
                                        {1, 5},
                                        {1, 9},
                                        {1, 10},
                                        {1, 13},
                                        {1, 14},
                                        {1, 16},
                                        {1, 20},
                                        {1, 23},
                                        {2, 2},
                                        {2, 3},
                                        {2, 4},
                                        {2, 7},
                                        {2, 8},
                                        {2, 9},
                                        {2, 18},
                                        {2, 19},
                                        {2, 20},
                                        {3, 0},
                                        {3, 2},
                                        {3, 6},
                                        {3, 10},
                                        {3, 11},
                                        {3, 12},
                                        {3, 13},
                                        {3, 18},
                                        {3, 19},
                                        {4, 5},
                                        {4, 7},
                                        {4, 10},
                                        {4, 11},
                                        {4, 13},
                                        {4, 14},
                                        {4, 16},
                                        {4, 19},
                                        {4, 20},
                                        {4, 22},
                                        {5, 4},
                                        {5, 5},
                                        {5, 8},
                                        {5, 9},
                                        {5, 14},
                                        {5, 16},
                                        {5, 23},
                                        {6, 3},
                                        {6, 6},
                                        {6, 7},
                                        {6, 9},
                                        {6, 10},
                                        {6, 11},
                                        {6, 12},
                                        {6, 13},
                                        {6, 22},
                                        {6, 23},
                                        {7, 1},
                                        {7, 5},
                                        {7, 8},
                                        {7, 10},
                                        {7, 12},
                                        {7, 13},
                                        {7, 15},
                                        {7, 22},
                                        {7, 23},
                                        {8, 0},
                                        {8, 1},
                                        {8, 8},
                                        {8, 9},
                                        {8, 11},
                                        {8, 14},
                                        {8, 18},
                                        {8, 21},
                                        {8, 23},
                                        {9, 0},
                                        {9, 2},
                                        {9, 5},
                                        {9, 8},
                                        {9, 9},
                                        {9, 10},
                                        {9, 11},
                                        {9, 14},
                                        {9, 15},
                                        {9, 17},
                                        {9, 19},
                                        {9, 21},
                                        {9, 22},
                                        {9, 23},
                                        {10, 0},
                                        {10, 1},
                                        {10, 2},
                                        {10, 3},
                                        {10, 4},
                                        {10, 5},
                                        {10, 7},
                                        {10, 8},
                                        {10, 10},
                                        {10, 14},
                                        {10, 17},
                                        {10, 18},
                                        {10, 21},
                                        {10, 22},
                                        {11, 1},
                                        {11, 2},
                                        {11, 3},
                                        {11, 4},
                                        {11, 5},
                                        {11, 7},
                                        {11, 8},
                                        {11, 10},
                                        {11, 12},
                                        {11, 16},
                                        {11, 17},
                                        {11, 18},
                                        {12, 0},
                                        {12, 1},
                                        {12, 5},
                                        {12, 7},
                                        {12, 9},
                                        {12, 11},
                                        {12, 12},
                                        {12, 13},
                                        {12, 15},
                                        {12, 18},
                                        {12, 19},
                                        {12, 22},
                                        {12, 23},
                                        {13, 1},
                                        {13, 2},
                                        {13, 3},
                                        {13, 4},
                                        {13, 10},
                                        {13, 11},
                                        {13, 13},
                                        {13, 16},
                                        {13, 17},
                                        {13, 22},
                                        {13, 23},
                                        {14, 0},
                                        {14, 3},
                                        {14, 4},
                                        {14, 5},
                                        {14, 6},
                                        {14, 8},
                                        {14, 10},
                                        {14, 11},
                                        {14, 12},
                                        {14, 15},
                                        {14, 16},
                                        {14, 17},
                                        {14, 18},
                                        {14, 20},
                                        {14, 22},
                                        {15, 1},
                                        {15, 4},
                                        {15, 5},
                                        {15, 6},
                                        {15, 8},
                                        {15, 10},
                                        {15, 12},
                                        {15, 14},
                                        {15, 15},
                                        {15, 17},
                                        {15, 19},
                                        {15, 20},
                                        {16, 1},
                                        {16, 4},
                                        {16, 6},
                                        {16, 7},
                                        {16, 8},
                                        {16, 10},
                                        {16, 13},
                                        {16, 14},
                                        {16, 16},
                                        {16, 17},
                                        {16, 19},
                                        {16, 20},
                                        {17, 0},
                                        {17, 1},
                                        {17, 5},
                                        {17, 7},
                                        {17, 15},
                                        {17, 18},
                                        {17, 19},
                                        {17, 23},
                                        {18, 1},
                                        {18, 3},
                                        {18, 4},
                                        {18, 5},
                                        {18, 6},
                                        {18, 7},
                                        {18, 8},
                                        {18, 9},
                                        {18, 11},
                                        {18, 12},
                                        {18, 14},
                                        {18, 15},
                                        {18, 17},
                                        {18, 18},
                                        {18, 22},
                                        {18, 23},
                                        {19, 0},
                                        {19, 1},
                                        {19, 4},
                                        {19, 10},
                                        {19, 11},
                                        {19, 22},
                                        {20, 1},
                                        {20, 2},
                                        {20, 3},
                                        {20, 4},
                                        {20, 5},
                                        {20, 6},
                                        {20, 7},
                                        {20, 8},
                                        {20, 9},
                                        {20, 16},
                                        {20, 20},
                                        {21, 0},
                                        {21, 1},
                                        {21, 4},
                                        {21, 5},
                                        {21, 6},
                                        {21, 8},
                                        {21, 11},
                                        {21, 15},
                                        {21, 17},
                                        {21, 18},
                                        {21, 20},
                                        {21, 21},
                                        {22, 6},
                                        {22, 7},
                                        {22, 8},
                                        {22, 9},
                                        {22, 14},
                                        {22, 15},
                                        {22, 18},
                                        {22, 21},
                                        {23, 0},
                                        {23, 1},
                                        {23, 2},
                                        {23, 3},
                                        {23, 4},
                                        {23, 8},
                                        {23, 9},
                                        {23, 10},
                                        {23, 11},
                                        {23, 14},
                                        {23, 16},
                                        {23, 18},
                                        {23, 19},
                                        {23, 20},
                                        {23, 21},
                                        {23, 22}};

        auto pixel = std::make_shared<PaiPixel>(paiDevice, 24, 24, walls);
        auto grid = PaiGameObject::createGameObject();
        grid.pixel = pixel;

        gameObjects.emplace(grid.getId(), std::move(grid));
    }
} // namespace pai
