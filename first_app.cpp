#include "first_app.h"
#include "simple_render_system.hpp"
#include "input_manager.hpp"


// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
#include<chrono>

namespace VULKI {


	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ vulkiDevice, vulkiRenderer.getSwapChainRenderPass() };
        VulkiCamera camera{};
        camera.setViewTarget(glm::vec3(0.f,0.f,2.f), glm::vec3(0.0f));

        auto viewObject = VulkiGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vulkiWindow.shouldClose()) {
			// track window events
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(vulkiWindow.getGLFWwindow(), frameTime, viewObject);
            camera.setViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);

            float aspect = vulkiRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);

			if (auto commandBuffer = vulkiRenderer.beginFrame()) {

				vulkiRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
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

		std::shared_ptr<VulkiModel> vulkiModel = VulkiModel::createModelFromFile(vulkiDevice, "D:\\Projects\\VulkiRender\\models\\smooth_vase.obj");
        auto cube = VulkiGameObject::createGameObject();
        cube.model = vulkiModel;
        cube.transform.translation = { .0f,.0f,0.f };
        cube.transform.scale = { .5f,.5f,.5f };

		gameObjects.push_back(std::move(cube));
	}


}  