#pragma once

#include "pai_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace pai
{

    class PaiDescriptorSetLayout
    {
    public:
        class Builder
        {
        public:
            Builder(PaiDevice &paiDevice) : paiDevice{paiDevice} {}

            Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<PaiDescriptorSetLayout> build() const;

        private:
            PaiDevice &paiDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        PaiDescriptorSetLayout(
            PaiDevice &paiDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~PaiDescriptorSetLayout();
        PaiDescriptorSetLayout(const PaiDescriptorSetLayout &) = delete;
        PaiDescriptorSetLayout &operator=(const PaiDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        PaiDevice &paiDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class PaiDescriptorWriter;
    };

    class PaiDescriptorPool
    {
    public:
        class Builder
        {
        public:
            Builder(PaiDevice &paiDevice) : paiDevice{paiDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<PaiDescriptorPool> build() const;

        private:
            PaiDevice &paiDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        PaiDescriptorPool(
            PaiDevice &paiDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~PaiDescriptorPool();
        PaiDescriptorPool(const PaiDescriptorPool &) = delete;
        PaiDescriptorPool &operator=(const PaiDescriptorPool &) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        PaiDevice &paiDevice;
        VkDescriptorPool descriptorPool;

        friend class PaiDescriptorWriter;
    };

    class PaiDescriptorWriter
    {
    public:
        PaiDescriptorWriter(PaiDescriptorSetLayout &setLayout, PaiDescriptorPool &pool);

        PaiDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        PaiDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        PaiDescriptorSetLayout &setLayout;
        PaiDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

} // namespace pai
