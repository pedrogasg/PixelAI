#pragma once

#include "../base/pai_camera.hpp"
#include "../base/pai_device.hpp"
#include "../base/pai_pipeline.hpp"
#include "../pai_game_object.hpp"
#include "../pai_frame_info.hpp"

// std
#include <memory>
#include <vector>

namespace pai
{
    class UnitSystem
    {
    public:
        UnitSystem(
            PaiDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~UnitSystem();

        UnitSystem(const UnitSystem &) = delete;
        UnitSystem &operator=(const UnitSystem &) = delete;

        void update(FrameInfo &frameInfo, GlobalUbo &ubo);
        void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        PaiDevice &paiDevice;

        std::unique_ptr<PaiPipeline> paiPipeline;
        VkPipelineLayout pipelineLayout;
    };
}