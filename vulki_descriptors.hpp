#pragma once

#include "vulki_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace VULKI {

    class VulkiDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(VulkiDevice& vulkiDevice) : vulkiDevice{ vulkiDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<VulkiDescriptorSetLayout> build() const;

        private:
            VulkiDevice& vulkiDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        VulkiDescriptorSetLayout(
            VulkiDevice& vulkiDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~VulkiDescriptorSetLayout();
        VulkiDescriptorSetLayout(const VulkiDescriptorSetLayout&) = delete;
        VulkiDescriptorSetLayout& operator=(const VulkiDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        VulkiDevice& vulkiDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class VulkiDescriptorWriter;
    };

    class VulkiDescriptorPool {
    public:
        class Builder {
        public:
            Builder(VulkiDevice& vulkiDevice) : vulkiDevice{ vulkiDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<VulkiDescriptorPool> build() const;

        private:
            VulkiDevice& vulkiDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        VulkiDescriptorPool(
            VulkiDevice& vulkiDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~VulkiDescriptorPool();
        VulkiDescriptorPool(const VulkiDescriptorPool&) = delete;
        VulkiDescriptorPool& operator=(const VulkiDescriptorPool&) = delete;

        bool allocateDescriptorSet(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        VulkiDevice& vulkiDevice;
        VkDescriptorPool descriptorPool;

        friend class VulkiDescriptorWriter;
    };

    class VulkiDescriptorWriter {
    public:
        VulkiDescriptorWriter(VulkiDescriptorSetLayout& setLayout, VulkiDescriptorPool& pool);

        VulkiDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        VulkiDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        VulkiDescriptorSetLayout& setLayout;
        VulkiDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace vulki