#include "ether.h"
#include <functional>

namespace ether {

	void engine_input_mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void engine_input_scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
	void engine_input_framebuffer_size_callback(GLFWwindow * window, int width, int height);

	Engine::Engine()
		: OpenglMajorVersion(3), OpenglMinorVersion(3) {
	}

	Engine::~Engine() {
	}

	void Engine::Init() {

		// glfw: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenglMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenglMinorVersion);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		Display.Init();
		glfwMakeContextCurrent(Display.getWindowHandle());

		InputsConfigure();

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw EngineError("Failed to initialize GLAD");
		}

		ShaderProgram.Init();

	}

	void Engine::Run() {

		while (!Display.ShouldClose()) {

			ProcessInput();

			Display.Clear();
			Render.Prepare();

			ShaderProgram.Use();

			for (auto it = Vaos.begin(); it != Vaos.end(); it++) {
				Render.Execute(*(it));
			}

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(Display.getWindowHandle());
			glfwPollEvents();
		}

		glfwTerminate();
	}

	void Engine::ProcessInput() {

		auto window = Display.getWindowHandle();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			Display.Close();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			//ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			//ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			//ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			//ether_camera_gl_process_keyboard(&camera, ETHER_CAMERA_MOVMENT_RIGHT, deltaTime);
		}
	}

	void Engine::InputsConfigure() {
		
		glfwSetFramebufferSizeCallback(Display.getWindowHandle(), engine_input_framebuffer_size_callback);
		glfwSetCursorPosCallback(Display.getWindowHandle(), engine_input_mouse_callback);
		glfwSetScrollCallback(Display.getWindowHandle(), engine_input_scroll_callback);
	}

	void Engine::inputMouseCallback(GLFWwindow* window, double xpos, double ypos) {

	}

	void Engine::inputScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

	}

	void Engine::inputFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void engine_input_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		Engine::getInstance().inputMouseCallback(window, xpos, ypos);
	}

	void engine_input_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Engine::getInstance().inputScrollCallback(window, xoffset, yoffset);
	}

	void engine_input_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Engine::getInstance().inputFramebufferSizeCallback(window, width, height);
	}
}
