#pragma once

#include "Vulki_window.h"
#include "vulki_pipeline.h"
#include "vulki_device.hpp"
#include "vulki_swap_chain.hpp"

#include<memory>
#include<vector>

namespace VULKI {

	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrames();

		VulkiWindow vulkiWindow{ WIDTH, HEIGHT, "Hello Vulkan!!" };
		VulkiDevice vulkiDevice{ vulkiWindow };
		VulkiSwapChain vulkiSwapChain{ vulkiDevice, vulkiWindow.getExtent() };
		std::unique_ptr<VulkiPipeline> vulkiPipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
	};
}  // namespace lve