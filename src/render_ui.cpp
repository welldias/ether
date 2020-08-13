#include "ether.h"

namespace ether {

	RenderUI::RenderUI() {

		uiToolTipShown = false;
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
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void RenderUI::Init(Display& display) {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		//ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(display.getWindowHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 330");


		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF("resources\\fonts\\OpenSans-Light.ttf",  16);
		io.Fonts->AddFontFromFileTTF("resources\\fonts\\OpenSans-Regular.ttf", 16);
		io.Fonts->AddFontFromFileTTF("resources\\fonts\\OpenSans-Light.ttf", 32);
		io.Fonts->AddFontFromFileTTF("resources\\fonts\\OpenSans-Regular.ttf", 11);
		io.Fonts->AddFontFromFileTTF("resources\\fonts\\OpenSans-Bold.ttf", 11);
		io.Fonts->Build();

		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(15, 15);
		style.WindowRounding = 5.0f;
		style.FramePadding = ImVec2(5, 5);
		style.FrameRounding = 4.0f;
		style.ItemSpacing = ImVec2(12, 8);
		style.ItemInnerSpacing = ImVec2(8, 6);
		style.IndentSpacing = 25.0f;
		style.ScrollbarSize = 15.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 3.0f;

		//style->Colors[ImGuiCol_Text] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		//style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.39f, 0.38f, 0.77f);
		//style->Colors[ImGuiCol_WindowBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.70f);
		//style->Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.58f);
		//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.92f);
		//style->Colors[ImGuiCol_Border] = ImVec4(0.84f, 0.83f, 0.80f, 0.65f);
		//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		//style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		//style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 1.00f, 0.40f, 0.78f);
		//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.47f);
		//style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		//style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.21f);
		//style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.91f, 0.00f, 0.78f);
		//style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 1.00f, 0.00f, 0.80f);
		//style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
		//style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
		//style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.99f, 1.00f, 0.22f, 0.86f);
		//style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_Header] = ImVec4(0.25f, 1.00f, 0.00f, 0.76f);
		//style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.86f);
		//style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.00f, 0.00f, 0.32f);
		//style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
		//style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
		//style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
		//style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_Tab] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		//style->Colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		//style->Colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		//style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
		//style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		//style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		//style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.33f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.29f, 0.33f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.29f, 0.33f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.29f, 0.33f, 0.34f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.37f, 0.39f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.26f, 0.27f, 1.00f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);

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

	void RenderUI::ShowPanelControl() {

		auto guiColumnWidth = 230.0f;

		auto fonts = ImGui::GetIO().Fonts;

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		//ImGuiStyle& style = ImGui::GetStyle();
		//style.FrameBorderSize = 1.0f;
		//style.WindowBorderSize = 1.0f;

		ImGuiIO& io = ImGui::GetIO();
		//ImVec2 mainWindowSize(io.DisplaySize.x, 52);
		//ImVec2 mainWindowPos(io.DisplaySize.x, 0);


		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(guiColumnWidth, io.DisplaySize.y));

		ImGui::Begin("Main###Debug", NULL, window_flags);

		// Digital Weather
		// ImGui::PushFont(ImGuiIO().Fonts->Fonts[2]);
		ImGui::PushFont(fonts->Fonts[2]);
		ImGui::TextUnformatted("Digital Weather");
		ImGui::PopFont();

		ImGui::Text("FPS %.3f", 46.87f);

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Weather Level");
		ImGui::PopFont();

		SliderFromParam("globalNoiseLevel");
		SliderFromParam("hardwareUpdateIntervalFps");

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Temperature");
		ImGui::PopFont();

		RangeFromParams("kelvinWarmRange");
		SliderFromParam("temperatureSpeed");
		SliderFromParam("temperatureSpread");

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Intensity");
		ImGui::PopFont();

		RangeFromParams("intensityRangeFrom");

		SliderFromParam("intensitySpeed");
		SliderFromParam("intensitySpread");

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Presets");
		ImGui::PopFont();

		ImGui::PushItemWidth(195);

		bool updatePresetItem = false;

		char strSaveFileName[200] = { 0 };

		ImGui::InputText("##PresetSaveFileName", strSaveFileName, 128, ImGuiInputTextFlags_CallbackCharFilter,
			[](ImGuiTextEditCallbackData* data) { if (strchr("\\/%&.:<>|", (char)data->EventChar)) return 1; return 0; }
		);

		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (strlen(strSaveFileName) > 0) {
			if (ImGui::Button("Save", ImVec2(50, 25))) {
				updatePresetItem = true;
			}
		}
		else {
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Button("Save", ImVec2(50, 25));
			ImGui::PopStyleVar();
		}

		int presetComboItem = 0;
		const char* const items[] = {
			{"Value 1"},
			{"Value 2"},
			{"Value 3"},
			{"Value 4"},
		};


		ImGui::PushItemWidth(195);
		ImGui::Combo("##PresetLoadFileName", &presetComboItem, items, 4);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (presetComboItem > -1) {
			if (ImGui::Button("Load", ImVec2(50, 25))) {
				std::cout << "VENTO 01" << std::endl;
			}
		}
		else {
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
			ImGui::Button("Load", ImVec2(50, 25));
			ImGui::PopStyleVar();
		}

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Movement");
		ImGui::PopFont();
		bool mirrorCameraVal = false;
		if (ImGui::Checkbox("Mirrored", &mirrorCameraVal)) {
			std::cout << "VENTO 02" << std::endl;
		}
		SliderFromParam("offsetScale");

		float fdd[2] = { 0, 1 };

		ImGui::InputFloat2("Offset", fdd, 2);

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - guiColumnWidth, 0));
		ImGui::SetNextWindowSize(ImVec2(guiColumnWidth, io.DisplaySize.y));
		ImGui::Begin("Node Editor", NULL, window_flags);

		ImGui::PushFont(fonts->Fonts[2]);
		ImGui::Text("Node %i", 1);
		ImGui::PopFont();

		ImGui::PushFont(fonts->Fonts[3]);
		ImGui::Text("Software version: %i.%i", 3, 2);
		ImGui::PopFont();

		ImGui::PushItemWidth(195 - 60);

		InputFromParam("ownID");
		ImGui::SameLine();
		if (ImGui::Button("Save in node##doSaveId")) {
			std::cout << "VENTO 03" << std::endl;
		};

		InputFromParam("remoteID");

		ImGui::PopItemWidth();

		ImGui::PushFont(fonts->Fonts[3]);
		ImGui::TextUnformatted("Movement sensor selected");
		ImGui::PopFont();

		//  ImGui::TextUnformatted([guiLedSynth->peripheral identifier]);

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Ranges");
		ImGui::PopFont();
		ImGui::Text("Intensity: %.1f %%  ...  %.1f %%", 0.8f, 1.1f);
		ImGui::Text("Temperature: %i k  ...  %i k", 9.2f, 10.2f);
		bool useRanges = false;
		if (ImGui::Checkbox("Use Ranges", &useRanges)) {
			std::cout << "VENTO 04" << std::endl;
		}

		bool remoteOverride = true;
		if (ImGui::Checkbox("Override", &remoteOverride)) {
			std::cout << "VENTO 05" << std::endl;
		}

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Faders");
		ImGui::PopFont();
		SliderFromParam("intensityFader");
		SliderFromParam("temperatureFader");

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Remote");
		ImGui::PopFont();
		SliderFromParam("intensityRemote");
		SliderFromParam("temperatureRemote");
		SliderFromParam("mixRemote");

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Weather");
		ImGui::PopFont();
		SliderFromParam("intensityNoise");
		SliderFromParam("temperatureNoise");
		SliderFromParam("mixNoise");

		ImGui::PushFont(fonts->Fonts[1]);
		ImGui::TextUnformatted("Output");
		ImGui::PopFont();
		SliderFromParam("intensityOutput");
		SliderFromParam("temperatureOutput");

		if (uiToolTipShown) {
			ImGui::BeginTooltip();
			float percent = 2.5f;

			ImGui::PushFont(fonts->Fonts[3]);
			ImGui::Text("ID: %i\nVersion: %i.%i", 10, 3, 2);
			ImGui::Text("Remote %i selected", 123);

			ImGui::PushFont(fonts->Fonts[4]);
			ImGui::TextUnformatted("Faders");
			ImGui::PopFont();

			ImGui::Text("Intensity: %.1f %%\nTemperature: %i k", percent, 23);

			ImGui::PushFont(fonts->Fonts[4]);
			ImGui::TextUnformatted("Remote");
			ImGui::PopFont();

			ImGui::Text("Channel: %i\nIntensity: %.1f %%\nTemperature: %i k\nMix: %.1f %%", 11, percent, 43, 3.1f);

			ImGui::PushFont(fonts->Fonts[4]);
			ImGui::TextUnformatted("Weather");
			ImGui::PopFont();
			ImGui::Text("Intensity: %.1f %%\nTemperature: %i k\nMix: %.1f %%", percent, 221, 13.0f);

			ImGui::PushFont(fonts->Fonts[4]);
			ImGui::TextUnformatted("Output");
			ImGui::PopFont();
			ImGui::Text("Intensity: %.1f %%\nTemperature: %i k", percent, 28);

			ImGui::PushFont(fonts->Fonts[4]);
			ImGui::TextUnformatted("Movement Sensor");
			ImGui::PopFont();
			ImGui::Text("Sensor: %s\nLevel: %.1f %%", "activity", 23.21);

			ImGui::PushFont(fonts->Fonts[4]);
			ImGui::TextUnformatted("Light Sensor");
			ImGui::PopFont();
			ImGui::Text("Intensity: %i lux\nTemperature: %i k\nLevel: %i", 2, 3, 5);
			ImGui::PopFont();

			ImGui::EndTooltip();
		}

		ImGui::End();
	}

	void RenderUI::SliderFromParam(std::string title) {
		float valueFrom = 0;
		float valueTo = 0;
		float min = 0.0f;
		float max = 100.0f;

		if (ImGui::DragFloatRange2(title.c_str(), &valueFrom, &valueTo, 0.1f, min, max)) {
			std::cout << "SliderFromParam" << std::endl;
		}
	}

	void RenderUI::RangeFromParams(std::string title) {
		int valueFrom = 0;
		int valueTo = 0;
		int min = 0;
		int max = 100;

		if (ImGui::DragIntRange2(title.c_str(), &valueFrom, &valueTo, 1.0, min, max)) {
			std::cout << "RangeFromParams" << std::endl;
		}
	}

	void RenderUI::InputFromParam(std::string title) {

		float value = 0;

		if (ImGui::InputFloat(title.c_str(), &value)) {
			std::cout << "InputFromParam" << std::endl;
		}
		return;
	}

	void RenderUI::ShowToolBar() {

		ImGuiIO& io = ImGui::GetIO();

		ImVec2 mainWindowSize(io.DisplaySize.x, 52);
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

			//ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			//ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			//ImGui::Image((void*)(intptr_t)logoImageID, ImVec2((float)50, (float)49), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);

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
		style.WindowRounding = 0;

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

	void RenderUI::ShowMiscSettings() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 window_pos(10, 35);

		Display& display = Engine::getInstance()->Display;
		Camera& camera = Engine::getInstance()->Camera;

		ImGui::SetNextWindowSize(ImVec2(330, 500), ImGuiCond_Appearing);
		ImGui::SetNextWindowSizeConstraints(ImVec2(330, 0), ImVec2(330, display.FramebufferHeight / 1.3f));
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15, 0.15, 0.15, 0.85));

		if (ImGui::Begin("Misc Settings##window", &miscSettingsShown, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing))
		{

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, style.ItemSpacing.y * 2.0));
			if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen)) {

				//ImGui::PushItemWidth();
				ImGui::DragFloat("Position - X", &(camera.Position[0]), 0.1, -1, 1);
				ImGui::DragFloat("Position - Y", &(camera.Position[1]), 0.1, -1, 1);
				ImGui::DragFloat("Position - Z", &(camera.Position[2]), 0.1, -1, 1);
				//ImGui::PopItemWidth();
				//ImGui::PushItemWidth();
				ImGui::DragFloat("Up - X", &(camera.WorldUp[0]), 0.1, -1, 1);
				ImGui::DragFloat("Up - Y", &(camera.WorldUp[1]), 0.1, -1, 1);
				ImGui::DragFloat("Up - Z", &(camera.WorldUp[2]), 0.1, -1, 1);

				ImGui::DragFloat("Yaw", &(camera.Yaw), 5, -100.0f, 100.0f);
				ImGui::DragFloat("Pitch", &(camera.Pitch), 5, -89.0f, 89.0f);

				camera.Update();

				//ImGui::PopItemWidth();

				//ImGui::DragFloat("Rotation Speed", &cam.rotation_speed, 0.5, 0);
				//ImGui::DragFloat("Movement Speed", &cam.move_speed, 0.5, 0);
				//ImGui::PopItemWidth();
				//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
				//
				//ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0 - ImGui::CalcTextSize("View-To-World Matrix").x / 2.0 - ImGui::GetFrameHeight());
				//ImGui::Text("View-To-World Matrix");
				//ImGui::SameLine();
				//showHelpMarker("Set the Columns of view-to-world matrix. The columns represent the basis vectors, side, up, -lookAt (right-handed) in order. The 4th Column represents the translation vector i.e. the Camera Origin. ");
				//
				//ImGui::PushItemWidth(260);
				//float* mat = glm::value_ptr(cam.view2world_mat);
				//ImGui::DragFloat4("C1-X", mat, 0.02, -1.0, 1.0);
				//ImGui::DragFloat4("C2-Y", &mat[4], 0.02, -1.0, 1.0);
				//ImGui::DragFloat4("C3-Z", &mat[8], 0.02, -1.0, 1.0);
				//ImGui::DragFloat4("C4-T", &mat[12], 0.02);
				//ImGui::PopItemWidth();
				//
				//ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0 - (ImGui::CalcTextSize("Set Matrix").x + ImGui::CalcTextSize("Reset Camera").x + style.ItemSpacing.x) / 2.0);
				//if (ImGui::Button("Set Matrix"))
				//	cam.updateViewMatrix();
				//ImGui::SameLine();
				//if (ImGui::Button("Reset Camera"))
				//	cam.resetCamera();
			}

			//Renderer Settings
			if (ImGui::CollapsingHeader("Renderer Settings", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
			{
				//ImGui::Checkbox("Enable Post-Processing", &do_postproc);
				//ImGui::Checkbox("Global Illumination", &gi_check);
				//ImGui::SameLine();
				//showHelpMarker("A handy check for toggling GI. This will be passed on to the kernel, so make sure the kernel has the argument for it.");
				//ImGui::Checkbox("60-FPS Cap", &cap_fps);
				//ImGui::SameLine();
				//showHelpMarker("Checking it causes FPS to cap to 60. Only useful if your raytracer/pathtracer gives FPS greater than 60 (Goodluck with that xD).");
				//
				//unsigned long min_samples = 0;
				//ImGui::PushItemWidth(120);
				//ImGui::DragScalar("Save At Samples", ImGuiDataType_U32, &renderer.save_at_samples, 0.5, &min_samples);
				//if (ImGui::InputText("Save Filename", input_fn, 256, ImGuiInputTextFlags_AutoSelectAll))
				//	renderer.save_samples_fn = std::string(input_fn);
				//ImGui::PopItemWidth();
				//renderExtBox();
				//ImGui::Checkbox("Save Image as Screencap", &renderer.save_editor);
				//ImGui::SameLine();
				//showHelpMarker("Checking this causes \"Save At Samples\" to work as screen capture and saves Editor to the image as well.");
			}

			//BVH Settings
			if (ImGui::CollapsingHeader("BVH Settings", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
			{
				//if (renderer_start)
				//{
				//	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, renderer_start);
				//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				//}
				//
				//ImGui::Checkbox("Load BVH", &load_bvh);
				//ImGui::PushItemWidth(120);
				//ImGui::DragInt("Bins", &bvh_bins, 0.5, 1, 256);
				//ImGui::SameLine();
				//showHelpMarker("Set the number of bins/buckets to use in SAH based BVH construction. Set Values lower  than 3 to use the Median Splitting Technique.");
				//ImGui::PopItemWidth();
				//if (ImGui::Button("Create BVH"))
				//{
				//	renderer.render_scene.loadBVH(bvh_bins);
				//	update_bvh_buffer = true;
				//}
				//if (renderer_start)
				//{
				//	ImGui::PopItemFlag();
				//	ImGui::PopStyleVar();
				//}
			}

			//Kernel Settings
			if (ImGui::CollapsingHeader("Kernel Settings", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
			{
				//if (renderer_start)
				//{
				//	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, renderer_start);
				//	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				//}
				//ImGui::PushItemWidth(125);
				//
				//if (ImGui::DragInt("##Render Kernel BlocksX", &renderer.blocks.x, 0.2, 1, 100, "Blocks in X: %d"))
				//	renderer.updateKernelWGSize();
				//ImGui::SameLine();
				//if (ImGui::DragInt("##Render Kernel BlocksY", &renderer.blocks.y, 0.2, 1, 100, "Blocks in Y: %d"))
				//	renderer.updateKernelWGSize();
				//ImGui::SameLine();
				//showHelpMarker("You can divide the Image in blocks to allow the same rendering kernel to launch multiple times for a single frame. "
				//	"This decreases overall fps but also decreases execution time for 1 kernel instance which can be useful to avoid laggy GUI. Default 2x2=4");
				//
				//size_t max_val, min_val;
				//min_val = 0;
				//max_val = std::numeric_limits<unsigned long>::max();
				//
				//ImGui::DragScalar("##RKGWSX", ImGuiDataType_U32, &renderer.rk_gws[0], 0.2, &min_val, &max_val, "RK GWS X: %d");
				//ImGui::SameLine();
				//ImGui::DragScalar("##RKGWSY", ImGuiDataType_U32, &renderer.rk_gws[1], 0.2, &min_val, &max_val, "RK GWS Y: %d");
				//ImGui::SameLine();
				//showHelpMarker("Global Workgroup Size for each Rendering Kernel instance in X and Y");
				//
				//ImGui::DragScalar("##PPKGWSX", ImGuiDataType_U32, &renderer.ppk_gws[0], 0.2, &min_val, &max_val, "PPK GWS X: %d");
				//ImGui::SameLine();
				//ImGui::DragScalar("##PPKGWSY", ImGuiDataType_U32, &renderer.ppk_gws[1], 0.2, &min_val, &max_val, "PPK GWS Y: %d");
				//ImGui::SameLine();
				//showHelpMarker("Global Workgroup Size for the Post-Processing Kernel in X and Y");
				//
				//ImGui::DragScalar("##RKLWSX", ImGuiDataType_U32, &renderer.rk_lws[0], 0.2, &min_val, &max_val, "RK LWS X: %d");
				//ImGui::SameLine();
				//ImGui::DragScalar("##RKLWSY", ImGuiDataType_U32, &renderer.rk_lws[1], 0.2, &min_val, &max_val, "RK LWS Y: %d");
				//ImGui::SameLine();
				//showHelpMarker("Local Workgroup Size for each Rendering Kernel instance in X and Y. Set to 0 to let OpenCL find a size automatically.");
				//
				//ImGui::DragScalar("##PPKLWSX", ImGuiDataType_U32, &renderer.ppk_lws[0], 0.2, &min_val, &max_val, "PPK LWS X: %d");
				//ImGui::SameLine();
				//ImGui::DragScalar("##PPKLWSY", ImGuiDataType_U32, &renderer.ppk_lws[1], 0.2, &min_val, &max_val, "PPK LWS Y: %d");
				//ImGui::SameLine();
				//showHelpMarker("Local Workgroup Size for the Post-Processing Kernel in X and Y. Set to 0 to let OpenCL find a size automatically.");
				//
				//ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0 - ImGui::CalcTextSize("Reset Kernel Settings").x / 2.0);
				//if (ImGui::Button("Reset Kernel Settings"))
				//	renderer.updateKernelWGSize(true);
				//
				//if (renderer_start)
				//{
				//	ImGui::PopItemFlag();
				//	ImGui::PopStyleVar();
				//}
				//ImGui::PopItemWidth();
			}
			ImGui::PopStyleVar();
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
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
						//std::string win_size = std::to_string(supported_sizes[i].x) + "x" + std::to_string(supported_sizes[i].y);
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
