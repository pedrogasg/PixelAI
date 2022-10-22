#pragma once

#include "pai_device.hpp"
#include "pai_swap_chain.hpp"
#include "pai_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace pai
{
    class PaiRenderer
    {
    public:
        PaiRenderer(PaiWindow &window, PaiDevice &device);
        ~PaiRenderer();

        PaiRenderer(const PaiRenderer &) = delete;
        PaiRenderer &operator=(const PaiRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return paiSwapChain->getRenderPass(); }
        float getAspectRatio() const { return paiSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        PaiWindow &paiWindow;
        PaiDevice &paiDevice;
        std::unique_ptr<PaiSwapChain> paiSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex{0};
        int currentFrameIndex{0};
        bool isFrameStarted{false};
    };
}