#include "ether.h"

namespace ether {

	const unsigned int WIDTH  = 800;
	const unsigned int HEIGHT = 600;

	Display::Display() 
		: Width(WIDTH), Height(HEIGHT) {

		this->TitleWindow = "Ether";
		this->window = NULL;
		this->Mode = Mode::Windows;
	}

	Display::~Display() {

	}

	void Display::Init() {

		if (window != NULL)
			return; 

		window = glfwCreateWindow(Width, Height, TitleWindow.c_str(), NULL, NULL);
		if (window == NULL)
		{
			glfwTerminate();
			throw EngineError("Failed to create GLFW window");
		}
	}

	void Display::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(
			static_cast<GLfloat>(BackGroundColor.r),
			static_cast<GLfloat>(BackGroundColor.g),
			static_cast<GLfloat>(BackGroundColor.b),
			static_cast<GLfloat>(BackGroundColor.a));
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
}
