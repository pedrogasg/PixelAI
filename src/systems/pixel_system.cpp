#include "pixel_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace pai
{

    struct PixelColor
    {
        glm::vec4 color{};
    };

    PixelSystem::PixelSystem(
        PaiDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : paiDevice{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    PixelSystem::~PixelSystem()
    {
        vkDestroyPipelineLayout(paiDevice.device(), pipelineLayout, nullptr);
    }

    void PixelSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT| VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PixelColor);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(paiDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void PixelSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        PaiPipeline::pixelPipelineConfigInfo(pipelineConfig);
        PaiPipeline::enableAlphaBlending(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        paiPipeline = std::make_unique<PaiPipeline>(
            paiDevice,
            "shaders/pixel.vert.spv",
            "shaders/pixel.geom.spv",
            "shaders/pixel.frag.spv",
            pipelineConfig);
    }

    void PixelSystem::render(
        FrameInfo &frameInfo)
    {
        paiPipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0,
            1,
            &frameInfo.globalDescriptorSet,
            0,
            nullptr);

        for (auto &kv : frameInfo.gameObjects)
        {
            auto &obj = kv.second;
            if (obj.pixel == nullptr)
                continue;
            PixelColor push{};
            push.color = {.1f, .8f, .1f, .5f};

            vkCmdPushConstants(
                frameInfo.commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(PixelColor),
                &push);
            obj.pixel->bind(frameInfo.commandBuffer);
            obj.pixel->draw(frameInfo.commandBuffer);
        }
    }

}