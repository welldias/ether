#ifndef __ETHER_RENDERUI_H__
#define __ETHER_RENDERUI_H__

namespace ether {

	class RenderUI
	{
	public:
		RenderUI();
		~RenderUI();

		void Init(Display& display);
		void StartFrame();
		void RenderFrame();
		
		void RendUI();
		void ShowMainContent();
		void ShowLeftBar();
		void ShowToolBar();

		void ShowBenchmarkWindow(unsigned long time_passed);
		void ShowHelpMarker(std::string desc);
		bool ShowMenu();
		void ShowMessageBox(std::string title, std::string msg, std::string log);

	private:
		GLuint logoImageID;

		bool uiBenchmarkShown;
		int uiBenchmarkWheight;

		std::vector<Vector2> supported_sizes;
		std::vector<std::string> supported_exts;
		imgui_addons::ImGuiFileBrowser file_dialog;
	};
}

#endif // __ETHER_RENDERUI_H__