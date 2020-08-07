#include "ether.h"

namespace ether {

	RenderUI::RenderUI() {
		uiBenchmarkShown = true;
		uiBenchmarkWheight = 25;

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

	RenderUI::~RenderUI() {

	}

	void RenderUI::Init(GLFWwindow* window) {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		//ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void RenderUI::ShowToolBar() {

		ImGuiIO& io = ImGui::GetIO();

		ImVec2 mainWindowSize(io.DisplaySize.x, 80);
		ImVec2 mainWindowPos(io.DisplaySize.x, 0);

		ImGuiStyle& style = ImGui::GetStyle();
		style.GrabRounding = 0;
		style.ChildBorderSize = 0;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

		ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always, ImVec2(1, 0));
		ImGui::SetNextWindowSize(mainWindowSize);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(90, 100, 100));

		if (ImGui::Begin("Main-ToolBar", NULL, window_flags))
		{
			ImGui::BeginGroup();

			ImVec2 button_sz(50, 50);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(90, 100, 100));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(120, 130, 130));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(120, 130, 130));
			for (int n = 0; n < 10; n++) {

				if (n > 0)
					ImGui::SameLine();

				ImGui::PushID(n);
				ImGui::Button("MTB", button_sz);
				ImGui::PopID();
			}
			ImGui::PopStyleColor(3);

			ImGui::EndGroup();
		}
		ImGui::End();
		ImGui::PopStyleColor();

		return;
	}

	void RenderUI::ShowLeftBar() {

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		ImVec2 regionAvail = ImGui::GetContentRegionAvail();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)ImColor(243, 243, 243));

		ImGui::BeginChild("Main-LeftBar", ImVec2(250, regionAvail.y), true, window_flags);
		ImGui::EndChild();

		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)ImColor(255, 255, 255));
		ImGui::BeginChild("Main-AuxLeftBar", ImVec2(350, regionAvail.y), true, window_flags);
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

	void RenderUI::ShowMainContent() {

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)ImColor(243, 243, 243));

		ImGui::BeginChild("Main-ContentArea", ImGui::GetContentRegionAvail(), true, window_flags);
		ImGui::EndChild();

		ImGui::PopStyleColor();
	}

	void RenderUI::RendUI() {

		ImGuiIO& io = ImGui::GetIO();

		ImVec2 mainWindowSize(0, 0);
		ImVec2 mainWindowPos(0, 0);

		ImGuiStyle& style = ImGui::GetStyle();
		style.GrabRounding = 0;
		style.ChildBorderSize = 0;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		//window_flags |= ImGuiWindowFlags_NoTitleBar;
		//window_flags |= ImGuiWindowFlags_NoScrollbar;
		//window_flags |= ImGuiWindowFlags_MenuBar;
		//window_flags |= ImGuiWindowFlags_NoCollapse;
		//window_flags |= ImGuiWindowFlags_NoNav;
		//window_flags |= ImGuiWindowFlags_NoBackground;
		//window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(200, 200, 200));
		ImGui::Begin("Hello, world!", NULL, window_flags);

		mainWindowSize.x = static_cast<float>(io.DisplaySize.x);
		mainWindowSize.y = static_cast<float>(io.DisplaySize.y);

		ImGui::SetWindowPos(mainWindowPos);
		ImGui::SetWindowSize(mainWindowSize);

		//showToolBar();
		//showLeftBar();
		//ImGui::SameLine();
		//showMainContent();

		ImGui::End();
		ImGui::PopStyleColor();
	}

	void RenderUI::StartFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RenderUI::RenderFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void RenderUI::ShowBenchmarkWindow(unsigned long time_passed)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos(io.DisplaySize.x - 10, 35);

		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(1, 0));
		ImGui::SetNextWindowSize(ImVec2(270, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.85f));
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

			uiBenchmarkWheight = 35 + static_cast<int>(ImGui::GetWindowHeight());
		}
		ImGui::End();
		ImGui::PopStyleColor();
	}

	void RenderUI::ShowHelpMarker(std::string desc) {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool RenderUI::ShowMenu() {

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

	void RenderUI::ShowMessageBox(std::string title, std::string msg, std::string log)
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
			ImVec2 text_size = ImGui::CalcTextSize(msg.c_str(), NULL, true, static_cast<float>(wrap_size));
			ImGui::PushTextWrapPos(static_cast<float>(wrap_size));
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() - text_size.x) / 2.0f);
			ImGui::Text(msg.c_str());
			ImGui::PopTextWrapPos();

			if (!log.empty())
			{
				ImVec2 text_size = ImGui::CalcTextSize(log.c_str(), NULL, true, static_cast<float>(wrap_size));
				float cw_width = text_size.x + style.ScrollbarSize + style.WindowPadding.x * 2.0f;
				ImGui::BeginChild("Error Log", ImVec2(cw_width, std::min(500.0f, text_size.y)), true);
				ImGui::PushTextWrapPos(static_cast<float>(wrap_size));
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (cw_width - style.ScrollbarSize - text_size.x) / 2.0f - style.WindowPadding.x);
				ImGui::TextUnformatted(log.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndChild();
			}

			ImGui::Separator();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
			ImGui::SetCursorPosX(static_cast<float>(ImGui::GetWindowWidth() / 2.0 - ImGui::GetFrameHeight() / 2));
			if (ImGui::Button("OK"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

}
