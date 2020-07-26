#include "ether.h"

namespace ether {

	Matrix4::Matrix4(const Matrix4& m1) {
		memcpy(m, m1.m, sizeof(m));
	}

	void  Matrix4::Identity() {
		Zero();
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	}

}
