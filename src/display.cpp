#include "ether.h"

namespace ether {

	const unsigned int WIDTH  = 1024;
	const unsigned int HEIGHT = 786;

	std::function<void(const Vector3&, float, float)> Display::CameraUpdateCallback;
	std::function<void(const std::string&, const std::string&, const std::string&)> Display::MessageCallback;

	Display::Display() 
		: OpenglMajorVersion(3), OpenglMinorVersion(3), Width(WIDTH), Height(HEIGHT) {

		glfwSetErrorCallback(ErrorCallback);
		
		if (!glfwInit())
			throw std::runtime_error("GLFW failed to initialize.");

		this->TitleWindow = "Ether";
		this->window = NULL;
		this->Mode = Mode::Windows;

		CreateWindow();
		InitImGui();
	}

	Display::~Display() {

		if (window) {
			glDeleteRenderbuffers(3, rboIDs);
			glDeleteFramebuffers(1, &fboID);
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		if (window) {
			glfwDestroyWindow(window);
		}

		glfwTerminate();
	}

	void Display::ShowWindow() {
		glfwRestoreWindow(window);
		glfwFocusWindow(window);
	}

	void Display::InitImGui() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		
		//ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void Display::CreateWindow() {

		// get Primary Monitor and default Video Mode
		primaryMonitor = glfwGetPrimaryMonitor();
		videoMode = glfwGetVideoMode(this->primaryMonitor);

		// Context Hints.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenglMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenglMinorVersion);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//FrameBuffer Hints.
		glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

		//Window Hints
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		window = glfwCreateWindow(Width, Height, TitleWindow.c_str(), NULL, NULL);
		if (window == NULL) {
			glfwTerminate();
			throw EngineError("Window creation failed");
		}

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw EngineError("Couldn't Initialize GLAD");
		if (!GLAD_GL_VERSION_3_3)
			throw EngineError("OpenGL version 3.3 not supported");

		glGetError();
		glfwSetWindowUserPointer(window, this);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCursorPosCallback(window, CursorPosCallback);
		glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);
		glfwIconifyWindow(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapInterval(0);
		glfwPollEvents();
	}

	bool Display::SetupGlBuffer() {
		//Delete any previous RenderBuffer/FrameBuffer. Note that delete calls silently ignore any unused names and 0's
		glDeleteRenderbuffers(4, rboIDs);
		glDeleteFramebuffers(1, &fboID);

		glGenFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);

		glGenRenderbuffers(4, rboIDs);

		glBindRenderbuffer(GL_RENDERBUFFER, rboIDs[0]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, framebufferWidth, framebufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rboIDs[0]);

		glBindRenderbuffer(GL_RENDERBUFFER, rboIDs[1]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, framebufferWidth, framebufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, rboIDs[1]);

		glBindRenderbuffer(GL_RENDERBUFFER, rboIDs[2]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, framebufferWidth, framebufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, rboIDs[2]);

		glBindRenderbuffer(GL_RENDERBUFFER, rboIDs[3]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, framebufferWidth, framebufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_RENDERBUFFER, rboIDs[3]);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
				MessageCallback("Framebuffer not complete. Error code: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", "FrameBuffer Incomplete!", "");
			else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
				MessageCallback("Framebuffer not complete. Error code: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", "FrameBuffer Incomplete!", "");
			else if (status == GL_FRAMEBUFFER_UNDEFINED)
				MessageCallback("Framebuffer not complete. Error code: GL_FRAMEBUFFER_UNDEFINED", "FrameBuffer Incomplete!", "");
			else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
				MessageCallback("Framebuffer not complete. Error code: GL_FRAMEBUFFER_UNSUPPORTED", "FrameBuffer Incomplete!", "");
			else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
				MessageCallback("Framebuffer not complete. Error code: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", "FrameBuffer Incomplete!", "");
			else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
				MessageCallback("Framebuffer not complete. Error code: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER", "FrameBuffer Incomplete!", "");
			else
				MessageCallback("Framebuffer not complete. Error code: " + std::to_string((int)status), "FrameBuffer Incomplete!", "");
			return false;
		}

		//glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClearColor(
			static_cast<GLfloat>(BackGroundColor.r),
			static_cast<GLfloat>(BackGroundColor.g),
			static_cast<GLfloat>(BackGroundColor.b),
			static_cast<GLfloat>(BackGroundColor.a));
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(GL_COLOR_ATTACHMENT2);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		return true;
	}

	void Display::Clear() {
#if 0
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(
			static_cast<GLfloat>(BackGroundColor.r),
			static_cast<GLfloat>(BackGroundColor.g),
			static_cast<GLfloat>(BackGroundColor.b),
			static_cast<GLfloat>(BackGroundColor.a));
#else
		glReadBuffer(GL_COLOR_ATTACHMENT3);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(
			static_cast<GLfloat>(BackGroundColor.r),
			static_cast<GLfloat>(BackGroundColor.g),
			static_cast<GLfloat>(BackGroundColor.b),
			static_cast<GLfloat>(BackGroundColor.a));

		glBlitFramebuffer(0, 0, framebufferWidth, framebufferHeight, 0, 0, framebufferWidth, framebufferHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
#endif
	}

	bool Display::ShouldClose() {
		if (window == NULL)
			return true;

		return glfwWindowShouldClose(window) == 0 ? false : true;
	}

	void Display::Close() {
		if(window != NULL)
			glfwSetWindowShouldClose(window, true);
	}

	void Display::ErrorCallback(int error, const char* msg) {
		std::cout << msg << std::endl;
	}

	void Display::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (ImGui::GetIO().WantCaptureKeyboard)
			return;
		Display* display = (Display*)glfwGetWindowUserPointer(window);
	}

	void Display::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		Display* display = (Display*)glfwGetWindowUserPointer(window);
	}

	void Display::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
		Display* display = (Display*)glfwGetWindowUserPointer(window);
	}

	void Display::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		Display* display = (Display*)glfwGetWindowUserPointer(window);

		glViewport(0, 0, width, height);

		display->framebufferWidth = width;
		display->framebufferHeight = height;
	}
}
