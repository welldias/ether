#include "ether.h"

namespace ether {


	Render::Render() {
		this->mode = Mode::Line;
	}

	void Render::Init() {

		glEnable(GL_DEPTH_TEST);
		
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		
		SetMode(mode);
	}

	void Render::Execute(const VertexArray& vao) {
		vao.Draw();
	}

	void Render::SetMode(Render::Mode mode) {
		this->mode = mode;

		int glMode = GL_FILL;
		switch (mode)
		{
		case Mode::Point:
			glMode = GL_POINT;
			break;
		case Mode::Line:
			glMode = GL_LINE;
			break;
		case Mode::Fill:
		default:
			glMode = GL_FILL;
			break;
		}

		glPolygonMode(GL_FRONT_AND_BACK, glMode);
	}

	Render::Mode Render::GetMode() {
		return mode;
	}

}
