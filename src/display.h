#ifndef _ETHER_DISPLAY_
#define _ETHER_DISPLAY_

namespace ether {

	class Display {
    public:
        typedef std::function<void(const std::string&, const std::string&, const std::string&)> MessageCallbackFunction;
        typedef std::function<void(int, int, int, int)> KeyCallbackFunction;
        typedef std::function<void(int, int, int)> MouseButtonCallbackFunction;
        typedef std::function<void(double, double)> MouseScrollCallbackFunction;
        typedef std::function<void(double, double)> MouseCursorCallbackFunction;
        typedef std::function<void(int, int)> FramebufferSizeCallbackFunction;

        Display();
        ~Display();

        void Init();
        void ShowWindow();
        void FullScreen(bool set);
        bool ShouldClose();
        void Close();
        void Clear();

        GLFWwindow* getWindowHandle() { return window; };

        void SetMessageCallbackFunction(MessageCallbackFunction cb) { messageCallback = cb; };
        void SetKeyCallbackFunction(KeyCallbackFunction cb) { keyCallback = cb; };

        void SetMouseButtonCallback(MouseButtonCallbackFunction cb) { mouseButtonCallback = cb; };
        void SetMouseScrollCallback(MouseScrollCallbackFunction cb) { mouseScrollCallback = cb; };
        void SetMouseCursorCallback(MouseCursorCallbackFunction cb) { mouseCursorCallback = cb; };
        void SetFramebufferSizeCallback(FramebufferSizeCallbackFunction cb) { framebufferSizeCallback = cb; };

    private:
        void CreateWindow();
        bool SetupGlBuffer();

        MessageCallbackFunction messageCallback;
        KeyCallbackFunction keyCallback;
        MouseButtonCallbackFunction mouseButtonCallback;
        MouseScrollCallbackFunction mouseScrollCallback;
        MouseCursorCallbackFunction mouseCursorCallback;
        FramebufferSizeCallbackFunction framebufferSizeCallback;

        static void GlfErrorCallback(int error, const char* msg);
        static void GlfKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void GlfMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void GlfScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        static void GlfCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void GlfFramebufferSizeCallback(GLFWwindow* window, int width, int height);

    public:
        enum class Mode {
            FullScreen = 0,
            Windows,
        };

        Mode Mode;
        unsigned int Width;
        unsigned int Height;
        std::string TitleWindow;
        Rgba BackGroundColor;
        unsigned int OpenglMajorVersion;
        unsigned int OpenglMinorVersion;

        int framebufferWidth;
        int framebufferHeight;

    private:

        GLuint fboID;
        GLuint rboIDs[4];

        GLFWwindow* window;
        GLFWmonitor* primaryMonitor;
        const GLFWvidmode* videoMode;
	};
}

#endif // _ETHER_DISPLAY_


