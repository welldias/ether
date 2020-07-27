#ifndef _ETHER_ENGINE_
#define _ETHER_ENGINE_

namespace ether {
    class Engine {
    public:
        static Engine& getInstance() {
            static Engine instance;
            return instance;
        }
        //Engine(Engine const&) = delete;
        void operator=(Engine const&) = delete;

        ~Engine();
        void Init();
        void Run();

    public:
        unsigned int OpenglMajorVersion;
        unsigned int OpenglMinorVersion;
        Camera Camera;
        Display Display;
        Render Render;
        ShaderProgram ShaderProgram;

        std::vector<Vao> Vaos;

        Engine();
        void InputsConfigure();
        void ProcessInput();

    public:
        void inputMouseCallback(GLFWwindow* window, double xpos, double ypos);
        void inputScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void inputFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    };

}

#endif // _ETHER_ENGINE_


