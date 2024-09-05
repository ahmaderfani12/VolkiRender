#pragma once

#include "vulki_window.h"
#include "vulki_camera.hpp"
#include "vulki_device.hpp"
#include "vulki_swap_chain.hpp"
#include "vulki_game_object.hpp"
#include "vulki_renderer.hpp"

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
		void loadGameObjects();

		VulkiWindow vulkiWindow{ WIDTH, HEIGHT, "Hello Vulkan!!" };
		VulkiDevice vulkiDevice{ vulkiWindow };
		VulkiRenderer vulkiRenderer{ vulkiWindow, vulkiDevice };
		std::vector<VulkiGameObject> gameObjects;
	};
}  // namespace lve