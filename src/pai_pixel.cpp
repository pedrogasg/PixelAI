#include "pai_pixel.hpp"

#include "pai_utils.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// std
#include <cassert>
#include <cstring>
#include <unordered_map>
#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace std
{
    template <>
    struct hash<pai::PaiPixel::Vertex>
    {
        size_t operator()(pai::PaiPixel::Vertex const &vertex) const
        {
            size_t seed = 0;
            pai::hashCombine(seed, vertex.position);
            return seed;
        }
    };
} // namespace std

namespace pai
{
    std::vector<VkVertexInputBindingDescription> PaiPixel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> PaiPixel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)});
        return attributeDescriptions;
    }

    PaiPixel::PaiPixel(PaiDevice &device, const int height, const int width) : paiDevice{device}
    {
        std::vector<Vertex> vertices;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < height; j++)
            {
                vertices.push_back({{i * 0.2, j * 0.2}});
            }
        }
        createVertexBuffers(vertices);
    }

    PaiPixel::~PaiPixel() {}

    void PaiPixel::createVertexBuffers(const std::vector<Vertex> &vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        uint32_t vertexSize = sizeof(vertices[0]);

        PaiBuffer stagingBuffer{
            paiDevice,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        stagingBuffer.map();
        stagingBuffer.writeToBuffer((void *)vertices.data());

        vertexBuffer = std::make_unique<PaiBuffer>(
            paiDevice,
            vertexSize,
            vertexCount,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        paiDevice.copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
    }

    void PaiPixel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer->getBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    }

    void PaiPixel::draw(VkCommandBuffer commandBuffer)
    {

        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

}