#pragma once

#include "lve_window.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_model.hpp"

#include <memory>

namespace lve
{
    class LveAppBase
    {
    private:
        LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        LveDevice lveDevice{lveWindow};
        LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
        std::unique_ptr<LvePipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<LveModel> lveModel;

        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
        void sierpinski(
            std::vector<LveModel::Vertex> &vertices,
            int depth,
            glm::vec2 left,
            glm::vec2 right,
            glm::vec2 top);

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        LveAppBase();
        ~LveAppBase();

        LveAppBase(const LveAppBase &) = delete;
        LveAppBase &operator=(const LveAppBase &) = delete;

        void run();
    };
}