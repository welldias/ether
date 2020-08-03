#ifndef _ETHER_DISPLAY_
#define _ETHER_DISPLAY_

namespace ether {
	class Display {
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


