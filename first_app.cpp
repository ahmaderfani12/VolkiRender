#include "first_app.h"

namespace VULKI {

	void FirstApp::run() {
		while (!vulkiWindow.shouldClose()) {
			// track window events
			glfwPollEvents();
		}
	}
}  // namespace lve