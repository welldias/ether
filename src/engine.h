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

    private:
        Engine();

        void Message(const std::string& msg, const std::string& title, const std::string& log);
        void KeyEvent(int key, int scancode, int action, int mods);
        void MousePositionEvent(double xpos, double ypos);
        void MouseScrollEvent(double xoffset, double yoffset);
        void MouseButtonEvent(int button, int action, int mods);
        void InputFramebufferSizeEvent(int width, int height);

    public:
        Light Light;
        Camera Camera;
        Display Display;
        Render Render;
        RenderUI RenderUI;
        ShaderProgram ShaderProgram;
        VertexArray* Vao;

        bool mouseButton1Pressed;

    private:
        static Engine* instance;

        static float deltaTime;
        static float lastFrame;
        static float lastX;
        static float lastY;
        static bool firstMouse;

    };
}

#endif // _ETHER_ENGINE_


