#include "unit_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <map>
#include <array>
#include <cassert>
#include <stdexcept>

namespace pai
{
    struct UnitPushConstants
    {
        glm::vec4 position{};
        glm::vec4 color{};
        float radius;
    };

    UnitSystem::UnitSystem(
        PaiDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
        : paiDevice{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    UnitSystem::~UnitSystem()
    {
        vkDestroyPipelineLayout(paiDevice.device(), pipelineLayout, nullptr);
    }

    void UnitSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(UnitPushConstants);

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

    void UnitSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        PaiPipeline::defaultPipelineConfigInfo(pipelineConfig);
        PaiPipeline::enableAlphaBlending(pipelineConfig);
        pipelineConfig.attributeDescriptions.clear();
        pipelineConfig.bindingDescriptions.clear();
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        paiPipeline = std::make_unique<PaiPipeline>(
            paiDevice,
            "shaders/unit.vert.spv",
            "shaders/unit.frag.spv",
            pipelineConfig);
    }

    void UnitSystem::update(FrameInfo &frameInfo, GlobalUbo &ubo)
    {
        auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * frameInfo.frameTime, {-.5f, -1.f, 0.f});
        int lightIndex = 0;
        for (auto &kv : frameInfo.gameObjects)
        {
            auto &obj = kv.second;
            if (obj.unitPoint == nullptr)
                continue;

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            // update light position
            obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

            // copy light to ubo
            ubo.unitPoints[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
            ubo.unitPoints[lightIndex].color = glm::vec4(obj.color, obj.unitPoint->lightIntensity);

            lightIndex += 1;
        }
        ubo.numLights = lightIndex;
    }

    void UnitSystem::render(FrameInfo &frameInfo)
    {
        std::map<float, PaiGameObject::id_t> sorted;
        for (auto &kv : frameInfo.gameObjects)
        {
            auto &obj = kv.second;
            if (obj.unitPoint == nullptr)
                continue;

            // calculate distance
            auto offset = frameInfo.camera.getPosition() - obj.transform.translation;
            float disSquared = glm::dot(offset, offset);
            sorted[disSquared] = obj.getId();
        }

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

        for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            // use game obj id to find light object
            auto &obj = frameInfo.gameObjects.at(it->second);

            UnitPushConstants push{};
            push.position = glm::vec4(obj.transform.translation, 1.f);
            push.color = glm::vec4(obj.color, obj.unitPoint->lightIntensity);
            push.radius = obj.transform.scale.x;

            vkCmdPushConstants(
                frameInfo.commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(UnitPushConstants),
                &push);
            vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
        }
    }

}