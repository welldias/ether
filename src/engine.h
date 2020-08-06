#ifndef _ETHER_ENGINE_
#define _ETHER_ENGINE_

namespace ether {
    class Engine {
    public:
        static Engine* getInstance() {
            if (instance == NULL) {
                instance = new Engine();
            }
            return instance;
        }

        //Engine(Engine const&) = delete;
        void operator=(Engine const&) = delete;

        ~Engine();
        void Init();
        void Run();

        void InputsConfigure();
        void ProcessInput();

    private:
        Engine();

        void StartFrame();
        void RenderFrame();
        static void SetMessage(const std::string& msg, const std::string& title, const std::string& log);

        void ShowBenchmarkWindow(unsigned long time_passed);
        void ShowHelpMarker(std::string desc);
        bool ShowMenu();
        void ShowMessageBox(std::string title, std::string msg, std::string log);

    public:
        Light Light;
        Camera Camera;
        Display Display;
        Render Render;
        ShaderProgram ShaderProgram;
        std::vector<Vao> Vaos;

    public:
        void inputMouseCallback(GLFWwindow* window, double xpos, double ypos);
        void inputScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void inputFramebufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        static Engine* instance;

        static float deltaTime;
        static float lastFrame;
        static float lastX;
        static float lastY;
        static bool firstMouse;

        static std::string messageUiMsg;
        static std::string messageUiTitle;
        static std::string messageUiLog;

        bool uiBenchmarkShown;
        int uiBenchmarkWheight;
        
        struct WindowSize { int width; int height; };
        std::vector<WindowSize> supported_sizes;
        std::vector<std::string> supported_exts;
        imgui_addons::ImGuiFileBrowser file_dialog;

    };

}

#endif // _ETHER_ENGINE_


