#pragma once

# include "vulki_device.hpp"
#include <string>
#include <vector>

namespace VULKI {

	// How different stages of pipeline works
	struct PipelineConfigInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};


	class VulkiPipeline {
	public:
		VulkiPipeline(
			VulkiDevice &device,
			const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& confingInfo );

		~VulkiPipeline();
		VulkiPipeline(const VulkiPipeline&) = delete;
		void operator=(const VulkiPipeline&) = delete;

		void bind(VkCommandBuffer comamndBuffer);

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline(const std::string& vertFilepath,
			const std::string& fragFilepath,
			const PipelineConfigInfo& confingInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VulkiDevice& vulkiDevice;
		// Followings are pointer
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
} 