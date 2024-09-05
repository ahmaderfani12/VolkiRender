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
		bool wasWindowResized() { return frameBufferResized; }
		void resetWindowResizedFlag() { frameBufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return window; }
		// Get called in device
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		static void framebufferResizedCallback(GLFWwindow* window, int width, int height);
		void init();

		int width;
		int height;
		bool frameBufferResized = false;

		std::string name;
		GLFWwindow *window;
	};

}
