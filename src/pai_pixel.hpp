#pragma once

#include "base/pai_buffer.hpp"
#include "base/pai_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace pai
{
    class PaiPixel
    {
    public:
        struct Vertex
        {
            glm::vec2 position{};
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        };

        PaiPixel(PaiDevice &device, const int height, const int width);
        ~PaiPixel();

        PaiPixel(const PaiPixel &) = delete;
        PaiPixel &operator=(const PaiPixel &) = delete;

        void draw(VkCommandBuffer commandBuffer);
        void bind(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        

        PaiDevice &paiDevice;

        std::unique_ptr<PaiBuffer> vertexBuffer;
        uint32_t vertexCount;
    };
}