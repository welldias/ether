#include "ether.h"
#include <functional>

#include <cglm\cglm.h>

namespace ether {

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	float lastX = 800 / 2.0f;
	float lastY = 600 / 2.0f;
	bool firstMouse = true;


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

		// tell GLFW to capture our mouse
		glfwSetInputMode(Display.getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw EngineError("Failed to initialize GLAD");
		}

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		ShaderProgram.Init();

		Camera.Configure(0.0f, 0.0f, 7.5f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);
	}

	void Engine::Run() {

		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		while (!Display.ShouldClose()) {

			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			ProcessInput();

			Display.Clear();
			Render.Prepare();

			ShaderProgram.Use();

			Matrix4 projection;
			Camera.Perspective((float)Display.Width / (float)Display.Height, 0.1f, 100.0f, projection);
			ShaderProgram.Uniform("projection", projection);

			Matrix4 view;
			Camera.ViewMatrix(view);
			ShaderProgram.Uniform("view", view);

			Matrix4 model;
			//Vector3 vecA(0.0f, 0.0f, 0.0f);
			Vector3 vecB(0.0f, -0.05f, 0.0f);
			model.Identity();
			//model.Translate(vecA);
			model.Rotate(MathUtil::Radian(10.0f * glfwGetTime()), vecB);
			ShaderProgram.Uniform("model", model);

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
			Camera.MoveForward((float)2.5 * deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			Camera.MoveBackward((float)2.5 * deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			Camera.MoveLeft((float)2.5 * deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			Camera.MoveRight((float)2.5 * deltaTime);
		}
	}

	void Engine::InputsConfigure() {
		
		glfwSetFramebufferSizeCallback(Display.getWindowHandle(), engine_input_framebuffer_size_callback);
		glfwSetCursorPosCallback(Display.getWindowHandle(), engine_input_mouse_callback);
		glfwSetScrollCallback(Display.getWindowHandle(), engine_input_scroll_callback);
	}

	void Engine::inputMouseCallback(GLFWwindow* window, double xpos, double ypos) {
		if (firstMouse)
		{
			lastX = (float)xpos;
			lastY = (float)ypos;
			firstMouse = false;
		}

		float xoffset = (float)xpos - lastX;
		float yoffset = lastY - (float)ypos;  // reversed since y-coordinates go from bottom to top

		lastX = (float)xpos;
		lastY = (float)ypos;

		Camera.PointTo(xoffset, yoffset, true);
	}

	void Engine::inputScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		Camera.Zoom((float)yoffset);
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
