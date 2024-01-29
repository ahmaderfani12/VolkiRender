#pragma once

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include <string>

namespace VULKI
{
	class VulkiWindow
	{
	public:
		VulkiWindow(int w, int h, std::string name);
		~VulkiWindow();

		VulkiWindow(const VulkiWindow &) = delete;
		VulkiWindow &operator=(const VulkiWindow &) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void init();

		const int width;
		const int height;

		std::string name;
		GLFWwindow *window;
	};

}
