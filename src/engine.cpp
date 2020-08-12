#include "ether.h"

namespace ether {

	Engine* Engine::instance = new Engine();

	// timing
	float Engine::deltaTime = 0.0f;	// time between current frame and last frame
	float Engine::lastFrame = 0.0f;
	float Engine::lastX = 800 / 2.0f;
	float Engine::lastY = 600 / 2.0f;
	bool  Engine::firstMouse = true;

	Engine::Engine() {
		Vao = NULL;
		mouseButton1Pressed = false;
	}

	Engine::~Engine() {
	}

	void Engine::Init() {

		Light.Position.Set(0, 0, 10);
		Light.Color.Set(1, 1, 1);

		Display.SetMessageCallbackFunction([this](const std::string& msg, const std::string& title, const std::string& log) { Message(msg, title, log); });
		Display.SetKeyCallbackFunction([this](int key, int scancode, int action, int mods) { KeyEvent(key, scancode, action, mods); });
		Display.SetMouseCursorCallback([this](double xpos, double ypos) { MousePositionEvent(xpos, ypos); });
		Display.SetMouseScrollCallback([this](double xoffset, double yoffset) { MouseScrollEvent(xoffset, yoffset); });
		Display.SetMouseButtonCallback([this](int button, int action, int mods) { MouseButtonEvent(button, action, mods); });
		Display.SetFramebufferSizeCallback([this](int width, int height) { InputFramebufferSizeEvent(width, height); });
		
		Display.Init();
		RenderUI.Init(Display);
		Render.Init();

		lastX = Display.Width / 2.0f;
		lastY = Display.Height / 2.0f;

		//InputsConfigure();

		ShaderProgram.Init();

		Camera.Configure(0.0f, 0.0f, 7.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);
	}

	void Engine::Message(const std::string& msg, const std::string& title, const std::string& log) {
	}

	void Engine::Run() {

		int frames = 0;
		double frameCounter = 0;
		bool render = false;

		float skipTicks = 16.66666f;
		float startTime = 0;
		float lastTime = (float)glfwGetTime();
		float unprocessedTime = 0;
		float passedTime = 0;

		Display.ShowWindow();

		while (!Display.ShouldClose()) {

			if ((glfwGetTime() - startTime) * 1000 < skipTicks)
				continue;

			startTime = static_cast<float>(glfwGetTime());
			deltaTime = passedTime = startTime - lastTime;
			lastTime = startTime;

			unprocessedTime++;
			frameCounter++;

			//ProcessInput();

			Display.Clear();

			//RenderUI.StartFrame();

			//RenderUI.ShowPanelControl();
			//RenderUI.ShowMiscSettings();
			//RenderUI.ShowToolBar();
			//RenderUI.ShowBenchmarkWindow(0);

			ShaderProgram.Use();

			//ShaderProgram.Uniform("lightPosition", Light.Position);
			//ShaderProgram.Uniform("lightColour", Light.Color);

			//Matrix4 projection;
			//Camera.Perspective((float)Display.Width / (float)Display.Height, 0.1f, 100.0f, projection);
			//ShaderProgram.Uniform("projection", projection);
			//
			//Matrix4 view;
			//Camera.ViewMatrix(view);
			//ShaderProgram.Uniform("view", view);
			//
			//Matrix4 model;
			////Vector3 vecA(0.0f, 0.0f, 0.0f);
			////Vector3 vecB(0.0f, -0.05f, 0.0f);
			//model.Identity();
			////model.Translate(vecA);
			////model.Rotate(MathUtil::Radian(10.0f * static_cast<float>(glfwGetTime())), vecB);
			//ShaderProgram.Uniform("model", model);

			Vao->Draw();
			//Render.Execute(*Vao);

			//RenderUI.RenderFrame();

			glfwSwapBuffers(Display.getWindowHandle());
			glfwPollEvents();
		}
	}

	bool is_fullscreen = false;
	void Engine::KeyEvent(int key, int scancode, int action, int mods) {

		if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS || action == GLFW_REPEAT))
			Display.Close();
		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
			Camera.MoveForward((float)2.5 * deltaTime);
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
			Camera.MoveBackward((float)2.5 * deltaTime);
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
			Camera.MoveLeft((float)2.5 * deltaTime);
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
			Camera.MoveRight((float)2.5 * deltaTime);
		if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
			is_fullscreen = !is_fullscreen;
			Display.FullScreen(is_fullscreen);
		}
	}

	void Engine::MousePositionEvent(double xpos, double ypos) {
		
		if (!mouseButton1Pressed) {
			firstMouse = true;
			return;
		}
		
		if (firstMouse) {
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

	void Engine::MouseScrollEvent(double xoffset, double yoffset) {
		Camera.Zoom((float)yoffset);
	}

	void Engine::InputFramebufferSizeEvent(int width, int height) {
	}

	void Engine::MouseButtonEvent(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			mouseButton1Pressed = (action == GLFW_PRESS);
		}
	}
}
