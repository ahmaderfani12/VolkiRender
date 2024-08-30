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
        VulkiCamera camera{};
        camera.setViewTarget(glm::vec3(0.f,0.f,2.f), glm::vec3(0.0f));

		while (!vulkiWindow.shouldClose()) {
			// track window events
			glfwPollEvents();

            float aspect = vulkiRenderer.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
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


    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<VulkiModel> createCubeModel(VulkiDevice& device, glm::vec3 offset) {
        std::vector<VulkiModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},  // 1
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},   // 2
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},    // 3
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},  // 1
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},    // 3
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},   // 4

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},   // 1
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},     // 3
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},    // 2
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},   // 1
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},    // 4
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},     // 3

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},  // 1
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},   // 2
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},    // 3
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},  // 1
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},    // 3
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},   // 4

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},   // 1
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},     // 3
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},    // 2
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},   // 1
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},    // 4
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},     // 3

            // nose face (blue)
            {{-.5f, -.5f, .5f}, {.1f, .1f, .8f}},   // 1
            {{.5f, -.5f, .5f}, {.1f, .1f, .8f}},    // 2
            {{.5f, .5f, .5f}, {.1f, .1f, .8f}},     // 3
            {{-.5f, -.5f, .5f}, {.1f, .1f, .8f}},   // 1
            {{.5f, .5f, .5f}, {.1f, .1f, .8f}},     // 3
            {{-.5f, .5f, .5f}, {.1f, .1f, .8f}},    // 4

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}}, // 1
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},   // 3
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},  // 2
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}}, // 1
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},  // 4
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},   // 3

        };


        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<VulkiModel>(device, vertices);
    }

	void FirstApp::loadGameObjects() {

        std::shared_ptr<VulkiModel> vulkiModel = createCubeModel(vulkiDevice, { .0f,.0f,.0f });
        auto cube = VulkiGameObject::createGameObject();
        cube.model = vulkiModel;
        cube.transform.translation = { .0f,.0f,0.f };
        cube.transform.scale = { .5f,.5f,.5f };

		gameObjects.push_back(std::move(cube));
	}


}  