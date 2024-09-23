#pragma once

#include "vulki_camera.hpp"

// lib
#include <vulkan/vulkan.h>

namespace VULKI {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VulkiCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		VulkiGameObject::Map& gameObjects;
	};
}  