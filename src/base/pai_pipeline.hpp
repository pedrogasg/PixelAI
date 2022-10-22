#pragma once

#include "pai_device.hpp"

#include <string>
#include <vector>

namespace pai
{

    struct PipelineConfigInfo
    {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;
        PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class PaiPipeline
    {
    private:
        PaiDevice &paiDevice;
        VkPipeline graphicsPipeline;

        static std::vector<char> readFile(const std::string &filename);
        void createGraphicsPipeline(
            const std::string &vertFilepath,
            const std::string &fragFilepath,
            const PipelineConfigInfo &configInfo);
        void createGraphicsPipeline(
            const std::string &vertFilepath,
            const std::string &geomFilepath,
            const std::string &fragFilepath,
            const PipelineConfigInfo &configInfo);
        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    public:
        PaiPipeline(PaiDevice &device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
        PaiPipeline(PaiDevice &device, const std::string &vertFilepath, const std::string &geomFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

        ~PaiPipeline();

        PaiPipeline(const PaiPipeline &) = delete;
        PaiPipeline &operator=(const PaiPipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void pixelPipelineConfigInfo(PipelineConfigInfo &configInfo);
        static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);
        static void enableAlphaBlending(PipelineConfigInfo &configInfo);
    };

}