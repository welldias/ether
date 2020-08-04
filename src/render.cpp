#include "ether.h"

namespace ether {


	Render::Render() {
		this->mode = Mode::Fill;
	}

	void Render::Init() {
		SetMode(mode);
	}

	void Render::Execute(const Vao& vao) {
		vao.Draw();
	}

	void Render::SetMode(Render::Mode mode) {
		this->mode = mode;

		int glMode = GL_FILL;
		switch (mode)
		{
		case ether::Render::Mode::Point:
			glMode = GL_POINT;
			break;
		case ether::Render::Mode::Line:
			glMode = GL_LINE;
			break;
		case ether::Render::Mode::Fill:
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
