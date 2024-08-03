#include "first_app.h"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>


namespace VULKI {


	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ vulkiDevice, vulkiRenderer.getSwapChainRenderPass() };

		while (!vulkiWindow.shouldClose()) {
			// track window events
			glfwPollEvents();
			
			if (auto commandBuffer = vulkiRenderer.beginFrame()) {

				vulkiRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				vulkiRenderer.endSwapChainRenderPass(commandBuffer);

				vulkiRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(vulkiDevice.device());
	}

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::loadGameObjects() {
		std::vector<VulkiModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };		

		auto vulkiModel = std::make_shared<VulkiModel>(vulkiDevice, vertices);
		
		auto triangle = VulkiGameObject::createGameObject();
		triangle.model = vulkiModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f , .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>(); // 90 degree

		gameObjects.push_back(std::move(triangle));
	}


}  