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

    public:
        unsigned int OpenglMajorVersion;
        unsigned int OpenglMinorVersion;
        Camera Camera;
        Display Display;
        Render Render;
        ShaderProgram ShaderProgram;

        std::vector<Vao> Vaos;

        void InputsConfigure();
        void ProcessInput();

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
    };

}

#endif // _ETHER_ENGINE_


