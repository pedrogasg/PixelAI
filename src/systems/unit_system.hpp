#pragma once

#include "../lve_camera.hpp"
#include "../lve_device.hpp"
#include "../lve_frame_info.hpp"
#include "../lve_game_object.hpp"
#include "../lve_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace lve
{
    class UnitSystem
    {
    public:
        UnitSystem(
            LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~UnitSystem();

        UnitSystem(const UnitSystem &) = delete;
        UnitSystem &operator=(const UnitSystem &) = delete;

        void update(FrameInfo &frameInfo, GlobalUbo &ubo);
        void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        LveDevice &lveDevice;

        std::unique_ptr<LvePipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
    };
}