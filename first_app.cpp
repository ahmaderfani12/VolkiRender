#include "first_app.h"
#include "simple_render_system.hpp"
#include "input_manager.hpp"
#include "vulki_buffer.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
#include<chrono>
#include<numeric>

namespace VULKI {

	struct GlobalUbo {
		glm::mat4 projectionView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };  // w is intensity
		glm::vec3 lightPosition{ -1.f };
		alignas(16) glm::vec4 lightColor{ 1.f };  // w is light intensity
	};

	FirstApp::FirstApp() {
		globalPool =
			VulkiDescriptorPool::Builder(vulkiDevice)
			.setMaxSets(VulkiSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkiSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	void FirstApp::run() {

		std::vector<std::unique_ptr<VulkiBuffer>> uboBuffers(VulkiSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<VulkiBuffer>(
				vulkiDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = VulkiDescriptorSetLayout::Builder(vulkiDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();
		
		std::vector<VkDescriptorSet> globalDescriptorSets(VulkiSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++) {
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			VulkiDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}


		SimpleRenderSystem simpleRenderSystem{
			vulkiDevice,
			vulkiRenderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout() };

        VulkiCamera camera{};
        camera.setViewTarget(glm::vec3(0.f,0.f,2.f), glm::vec3(0.0f));

        auto viewObject = VulkiGameObject::createGameObject();
		viewObject.transform.translation.z = -2.0f;
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

				int frameIndex = vulkiRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex]
				};
				// update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				vulkiRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				vulkiRenderer.endSwapChainRenderPass(commandBuffer);

				vulkiRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(vulkiDevice.device());
	}


	FirstApp::~FirstApp() {}



	void FirstApp::loadGameObjects() {

		std::shared_ptr<VulkiModel> vulkiModel = VulkiModel::createModelFromFile(vulkiDevice, "models/smooth_vase.obj");
        auto go1 = VulkiGameObject::createGameObject();
		go1.model = vulkiModel;
		go1.transform.translation = { -0.5f, 0.5f ,0.0f };
		go1.transform.scale = { 3,3,3 };

		gameObjects.push_back(std::move(go1));

		vulkiModel = VulkiModel::createModelFromFile(vulkiDevice, "models/flat_vase.obj");
		auto go2 = VulkiGameObject::createGameObject();
		go2.model = vulkiModel;
		go2.transform.translation = { 0.5f, 0.5f ,0.0f };
		go2.transform.scale = { 3,3,3 };

		gameObjects.push_back(std::move(go2));

		vulkiModel = VulkiModel::createModelFromFile(vulkiDevice, "models/quad.obj");
		auto floor = VulkiGameObject::createGameObject();
		floor.model = vulkiModel;
		floor.transform.translation = { 0.f, .5f, 0.f };
		floor.transform.scale = { 3.f, 1.f, 3.f };
		gameObjects.push_back(std::move(floor));


	}


}  