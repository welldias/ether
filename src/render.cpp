#include "ether.h"

namespace ether {

	void Render::Prepare() {
	}

	void Render::Execute(const Vao& vao) {
		vao.Draw();
	}

}
