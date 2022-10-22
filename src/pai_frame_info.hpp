#pragma once

#include "base/pai_camera.hpp"
#include "pai_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace pai
{

    struct UnitPoint
    {
        glm::vec4 position{}; // ignore w
        glm::vec4 color{};    // w is intensity
    };

    struct GlobalUbo
    {
        glm::vec2 agent{};
    };

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        PaiCamera &camera;
        VkDescriptorSet globalDescriptorSet;
        PaiGameObject::Map &gameObjects;
    };
} // namespace pai
