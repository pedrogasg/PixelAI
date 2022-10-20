#pragma once

#include "../pai_camera.hpp"
#include "../pai_device.hpp"
#include "../pai_frame_info.hpp"
#include "../pai_game_object.hpp"
#include "../pai_pipeline.hpp"

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