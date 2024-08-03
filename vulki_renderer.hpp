#pragma once

#include "Vulki_window.h"
#include "vulki_device.hpp"
#include "vulki_swap_chain.hpp"

#include<cassert>
#include<memory>
#include<vector>

namespace VULKI {

	class VulkiRenderer {
	public:

		VulkiRenderer(VulkiWindow &window, VulkiDevice &device);
		~VulkiRenderer();

		VulkiRenderer(const VulkiRenderer&) = delete;
		VulkiRenderer& operator=(const VulkiRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return vulkiSwapChain->getRenderPass(); };
		bool isFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const { 
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
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
		// for resizing window
		void recreateSwapChain();


		VulkiWindow& vulkiWindow;
		VulkiDevice& vulkiDevice;
		std::unique_ptr<VulkiSwapChain> vulkiSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 }; // [o, Max_Frames_In_Flight]
		bool isFrameStarted{ false };
	};
}  // namespace lve