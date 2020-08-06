#ifndef _ETHER_DISPLAY_
#define _ETHER_DISPLAY_

namespace ether {
	class Display {
    public:
        Display();
        ~Display();

        void ShowWindow();
        bool ShouldClose();
        void Close();
        void Clear();

        GLFWwindow* getWindowHandle() { return window; };

        static std::function<void(const Vector3&, float, float)> CameraUpdateCallback;
        static std::function<void(const std::string&, const std::string&, const std::string&)> MessageCallback;

    private:
        void InitImGui();
        void CreateWindow();
        bool SetupGlBuffer();

        static void ErrorCallback(int error, const char* msg);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

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


