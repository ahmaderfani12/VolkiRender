#pragma once

#include "Vulki_window.h"
#include "vulki_pipeline.h"
#include "vulki_device.hpp"

namespace VULKI {

	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		VulkiWindow vulkiWindow{ WIDTH, HEIGHT, "Hello Vulkan!!" };
		VulkiDevice vulkiDevice{ vulkiWindow };
		VulkiPipeline lvePipeline{ vulkiDevice,
			"shaders/simple_shader.vert.spv",
			"shaders/simple_shader.frag.spv",
			VulkiPipeline::defaultPipeLineConfigInfo(WIDTH,HEIGHT)};
	};
}  // namespace lve