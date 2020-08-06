#include "ether.h"

namespace ether {

	Engine* Engine::instance = new Engine();

	// timing
	float Engine::deltaTime = 0.0f;	// time between current frame and last frame
	float Engine::lastFrame = 0.0f;
	float Engine::lastX = 800 / 2.0f;
	float Engine::lastY = 600 / 2.0f;
	bool  Engine::firstMouse = true;

	// log messages
	std::string Engine::messageUiMsg;
	std::string Engine::messageUiTitle;
	std::string Engine::messageUiLog;


	void engine_input_mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void engine_input_scroll_callback(GLFWwindow * window, double xoffset, double yoffset);
	void engine_input_framebuffer_size_callback(GLFWwindow * window, int width, int height);

	Engine::Engine() {
		supported_sizes.push_back({ 600, 800 });
		supported_sizes.push_back({ 640, 480 });
		supported_sizes.push_back({ 800, 600 });
		supported_sizes.push_back({ 1024, 768 });
		supported_sizes.push_back({ 1280, 720 });
		supported_sizes.push_back({ 1280, 1024 });
		supported_sizes.push_back({ 1600, 900 });
		supported_sizes.push_back({ 1920, 1080 });

		supported_exts.push_back(".png");
		supported_exts.push_back(".jpg");
		supported_exts.push_back(".hdr");
	}

	Engine::~Engine() {
	}

	void Engine::Init() {

		// glfw: initialize and configure
		//glfwInit();
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenglMajorVersion);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenglMinorVersion);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		Light.Position.Set(0, 0, 10);
		Light.Color.Set(1, 1, 1);

		//Display.MessageCallback(std::bind(&(Engine::SetMessage), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		//Display.CameraUpdateCallback();

		//Display.Init();
		//glfwMakeContextCurrent(Display.getWindowHandle());

		lastX = Display.Width / 2.0f;
		lastY = Display.Height / 2.0f;

		//InputsConfigure();

		// tell GLFW to capture our mouse
		//glfwSetInputMode(Display.getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: load all OpenGL function pointers
		//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		//	throw EngineError("Failed to initialize GLAD");
		//}

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		ShaderProgram.Init();

		Camera.Configure(0.0f, 0.0f, 7.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);

		Render.Init();
	}

	void Engine::StartFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Engine::RenderFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Engine::SetMessage(const std::string& msg, const std::string& title, const std::string& log) {
		Engine::messageUiMsg = msg;
		Engine::messageUiTitle = title;
		Engine::messageUiLog = log;
	}

	void Engine::Run() {

		uiBenchmarkShown = true;

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

			StartFrame();
			
			ShowMenu();
#if 1
			ShaderProgram.Use();

			ShaderProgram.Uniform("lightPosition", Light.Position);
			ShaderProgram.Uniform("lightColour", Light.Color);

			Matrix4 projection;
			Camera.Perspective((float)Display.Width / (float)Display.Height, 0.1f, 100.0f, projection);
			ShaderProgram.Uniform("projection", projection);

			Matrix4 view;
			Camera.ViewMatrix(view);
			ShaderProgram.Uniform("view", view);

			Matrix4 model;
			//Vector3 vecA(0.0f, 0.0f, 0.0f);
			//Vector3 vecB(0.0f, -0.05f, 0.0f);
			model.Identity();
			//model.Translate(vecA);
			//model.Rotate(MathUtil::Radian(10.0f * static_cast<float>(glfwGetTime())), vecB);
			ShaderProgram.Uniform("model", model);

			for (auto it = Vaos.begin(); it != Vaos.end(); it++) {
				Render.Execute(*(it));
			}
#endif

			ShowBenchmarkWindow(frames);

			RenderFrame();

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(Display.getWindowHandle());
			glfwPollEvents();
		}

		//glfwTerminate();
	}

	void Engine::ShowBenchmarkWindow(unsigned long time_passed)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos(io.DisplaySize.x - 10, 35);

		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1, 0));
		ImGui::SetNextWindowSize(ImVec2(270, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1, 0.1, 0.1, 0.85));
		if (ImGui::Begin("Benchmark##window", &uiBenchmarkShown, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Rendering Kernel");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemSpacing.x);
			ImGui::TextWrapped("%s", "cl_manager.rk_file");

			ImGui::Text("Post-Proc Kernel");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": ");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().ItemSpacing.x);
			ImGui::TextWrapped("%s", "cl_manager.ppk_file");

			ImGui::Text("FPS");
			ImGui::SameLine();
			ShowHelpMarker("This is the number of frames processed by the GPU in one second.");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": %d", 987654321);

			ImGui::Text("ms/frame");
			ImGui::SameLine();
			ShowHelpMarker("This is the time taken by the GPU to render one whole frame. By default the image is broken in 4 blocks. Roughly equal to 4 times ms/rk. Additional overhead due to kernel launch, blitting etc.");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": %.2f ms", 9876.54321f);

			ImGui::Text("ms/rk");
			ImGui::SameLine();
			ShowHelpMarker("This is the average time taken by a single kernel instance. By default the image is broken in 4 blocks and a kernel is launched for each block.");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": %.2f ms", 9876.54321f);

			ImGui::Text("ms/ppk");
			ImGui::SameLine();
			ShowHelpMarker("This is the time taken by the post-processing kernel if loaded.");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": %.2f ms", 9876.54321f);

			ImGui::Text("Render Time");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": %lu sec", 987654321);

			ImGui::Text("Samples Taken");
			ImGui::SameLine();
			ImGui::SetCursorPosX(140);
			ImGui::Text(": %lu spp", 987654321);

			uiBenchmarkWheight = 35 + ImGui::GetWindowHeight();
		}
		ImGui::End();
		ImGui::PopStyleColor();
	}

	void Engine::ShowHelpMarker(std::string desc) {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool Engine::ShowMenu() {

		bool open_obj = false, save_fildialog = false, open_rk = false, open_ppk = false, open_about = false, open_usage = false, show_message = false;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load OBJ", NULL, false, !uiBenchmarkShown))
					open_obj = true;

				if (ImGui::MenuItem("Load Render Kernel", NULL, false, !uiBenchmarkShown))
					open_rk = true;

				if (ImGui::MenuItem("Load Post-Proc Kernel", NULL, false, !uiBenchmarkShown))
					open_ppk = true;

				if (ImGui::MenuItem("Reload Render Kernel", NULL, false, !uiBenchmarkShown))
				{
					show_message = true;
				}

				if (ImGui::MenuItem("Reload Post-Proc Kernel", NULL, false, !uiBenchmarkShown))
				{
					show_message = true;
				}

				if (ImGui::MenuItem("Reload Material File", NULL, false, !uiBenchmarkShown))
				{
					show_message = true; //Either success or failure.
				}

				if (ImGui::MenuItem("Save Image", NULL, false, uiBenchmarkShown))
					save_fildialog = true;

				ImGui::Separator();
				if (ImGui::MenuItem("Start", NULL, &uiBenchmarkShown, !uiBenchmarkShown))
				{
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Benchmark", NULL, &uiBenchmarkShown))
				{
						uiBenchmarkWheight = 25;
				}
				ImGui::MenuItem("Scene Info", NULL, &uiBenchmarkShown);
				ImGui::MenuItem("Misc Settings", NULL, &uiBenchmarkShown);

				if (ImGui::BeginMenu("Window Size", !uiBenchmarkShown))
				{
					for (int i = 0; i < supported_sizes.size(); i++)
					{
						std::string win_size = std::to_string(supported_sizes[i].width) + "x" + std::to_string(supported_sizes[i].height);
						//if (ImGui::MenuItem(win_size.c_str(), NULL, selected_size == i, !is_fullscreen))
						//{
						//	selected_size = i;
						//	update_image_buffer = true;
						//	glfwSetWindowSize(glfw_manager.window, supported_sizes[i].width, supported_sizes[i].height);
						//	renderer.updateKernelWGSize();
						//}
					}
					ImGui::Separator();
					if (ImGui::MenuItem("FullScreen", NULL, &uiBenchmarkShown))
					{
						//if (is_fullscreen)
						//{
						//	selected_size = -1;
						//	glfwSetWindowMonitor(glfw_manager.window, glfw_manager.prim_monitor, 0, 0, glfw_manager.mode->width, glfw_manager.mode->height, glfw_manager.mode->refreshRate);
						//}
						//else
						//{
						//	selected_size = 3;
						//	glfwSetWindowMonitor(glfw_manager.window, NULL, 200, 200, supported_sizes[3].width, supported_sizes[3].height, glfw_manager.mode->refreshRate);
						//}
						//update_image_buffer = true;
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About"))
					open_about = true;
				if (ImGui::MenuItem("Usage"))
					open_usage = true;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (open_obj)
			ImGui::OpenPopup("Open OBJ File");

		if (open_rk)
			ImGui::OpenPopup("Open Rendering Kernel File");

		if (open_ppk)
			ImGui::OpenPopup("Open Post-Processing Kernel File");

		if (save_fildialog)
			ImGui::OpenPopup("Save Image");

		if (file_dialog.showFileDialog("Open OBJ File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".obj,.rtt"))
		{
			show_message = true;
		}

		if (file_dialog.showFileDialog("Open Rendering Kernel File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".cl"))
		{
			show_message = true;
		}

		if (file_dialog.showFileDialog("Open Post-Processing Kernel File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".cl"))
		{
			show_message = true;
		}

		if (file_dialog.showFileDialog("Save Image", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".png,.jpg,.hdr"))
		{
		}

		if (open_about)
			ImGui::OpenPopup("About");
		ShowMessageBox("About", "Lorem ipsum", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");

		if (open_usage)
			ImGui::OpenPopup("Usage");
		ShowMessageBox("About", "Lorem ipsum", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua");

		return show_message;
	}

	void Engine::ShowMessageBox(std::string title, std::string msg, std::string log)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		int wrap_size;
		if (log.empty())
		{
			wrap_size = 380;
			ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_Appearing);
		}
		else
		{
			wrap_size = 460;
			ImGui::SetNextWindowSize(ImVec2(500, 0), ImGuiCond_Appearing);
		}

		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 0), ImVec2(500, 400));
		if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImVec2 text_size = ImGui::CalcTextSize(msg.c_str(), NULL, true, wrap_size);
			ImGui::PushTextWrapPos(wrap_size);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() - text_size.x) / 2.0f);
			ImGui::Text(msg.c_str());
			ImGui::PopTextWrapPos();

			if (!log.empty())
			{
				ImVec2 text_size = ImGui::CalcTextSize(log.c_str(), NULL, true, wrap_size);
				float cw_width = text_size.x + style.ScrollbarSize + style.WindowPadding.x * 2.0f;
				ImGui::BeginChild("Error Log", ImVec2(cw_width, std::min(500.0f, text_size.y)), true);
				ImGui::PushTextWrapPos(wrap_size);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cw_width - style.ScrollbarSize - text_size.x) / 2.0f - style.WindowPadding.x);
				ImGui::TextUnformatted(log.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndChild();
			}

			ImGui::Separator();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0 - ImGui::GetFrameHeight() / 2);
			if (ImGui::Button("OK"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
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
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS ) {
			int mode = static_cast<int>(Render.GetMode()) + 1;
			mode = mode % 3;
		
			Render.SetMode(static_cast<Render::Mode>(mode));
		}
	}

	void Engine::InputsConfigure() {
		
		glfwSetFramebufferSizeCallback(Display.getWindowHandle(), engine_input_framebuffer_size_callback);
		glfwSetCursorPosCallback(Display.getWindowHandle(), engine_input_mouse_callback);
		glfwSetScrollCallback(Display.getWindowHandle(), engine_input_scroll_callback);
	}

	void Engine::inputMouseCallback(GLFWwindow* window, double xpos, double ypos) {
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

	void Engine::inputScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		Camera.Zoom((float)yoffset);
	}

	void Engine::inputFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void engine_input_mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		Engine::getInstance()->inputMouseCallback(window, xpos, ypos);
	}

	void engine_input_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Engine::getInstance()->inputScrollCallback(window, xoffset, yoffset);
	}

	void engine_input_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Engine::getInstance()->inputFramebufferSizeCallback(window, width, height);
	}
}
