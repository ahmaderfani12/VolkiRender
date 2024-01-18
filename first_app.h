#pragma once

#include "Vulki_window.h"

namespace VULKI {

	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		VulkiWindow vulkiWindow{ WIDTH, HEIGHT, "Hello Vulkan!!" };
	};
}  // namespace lve