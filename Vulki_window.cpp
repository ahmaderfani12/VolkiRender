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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// forth(full screen) fifth(opengl context)
		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizedCallback);
	}

	void VulkiWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}
	 void VulkiWindow::framebufferResizedCallback(GLFWwindow* window, int width, int height) {
		 auto vulkiWindow = reinterpret_cast<VulkiWindow*>(glfwGetWindowUserPointer(window));
		 vulkiWindow->frameBufferResized = true;
		 vulkiWindow->width = width;
		 vulkiWindow->height = height;
	}

}
