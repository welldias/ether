#ifndef _ETHER_DISPLAY_
#define _ETHER_DISPLAY_

namespace ether {
	class Display {
    public:
        enum class Mode {
            FullScreen = 0,
            Windows,
        };

        Mode mode;
        unsigned int width;
        unsigned int height;
        std::string TitleWindow;
        Vector3 BackGroundColor;

    private:
        GLFWwindow* window;

    public:
        Display();
        ~Display();

        void Init();
        bool ShouldClose();
        void Close();
        void Clear();

        GLFWwindow* getWindowHandle() { return window; };
	};
}

#endif // _ETHER_DISPLAY_


