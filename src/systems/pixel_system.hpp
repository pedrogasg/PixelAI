
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
    class PixelSystem
    {
    public:
        PixelSystem(
            PaiDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PixelSystem();

        PixelSystem(const PixelSystem &) = delete;
        PixelSystem &operator=(const PixelSystem &) = delete;

         void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        PaiDevice &paiDevice;

        std::unique_ptr<PaiPipeline> paiPipeline;
        VkPipelineLayout pipelineLayout;
    };
}