
#pragma once

#include "../base/pai_camera.hpp"
#include "../base/pai_device.hpp"
#include "../pai_frame_info.hpp"
#include "../pai_game_object.hpp"
#include "../base/pai_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace pai
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(
            PaiDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

         void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        PaiDevice &paiDevice;

        std::unique_ptr<PaiPipeline> paiPipeline;
        VkPipelineLayout pipelineLayout;
    };
}