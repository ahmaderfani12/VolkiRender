#include "vulki_window.h"

#include <stdexcept>

namespace VULKI
{
	VulkiWindow::VulkiWindow(int w, int h, std::string name) : width{ w }, height{ h }, name{ name } {
		init();
	}

	VulkiWindow::~VulkiWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void VulkiWindow::init()
	{
		glfwInit();
		// Dont creat OpneGL contex
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// forth(full screen) fifth(opengl context)
		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	}

	void VulkiWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

}
