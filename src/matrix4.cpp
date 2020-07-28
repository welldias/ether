#include <cglm\cglm.h>

#include "ether.h"

namespace ether {

	Matrix4::Matrix4(const Matrix4& m1) {
		memcpy(m, m1.m, sizeof(m));
	}

	void  Matrix4::Identity() {
		memset(m, 0, sizeof(m));
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	}

	Matrix4& Matrix4::Translate(Vector3& v) {
		glm_translate(this->m, v.v);
		return *this;
	}

	Matrix4& Matrix4::Rotate(float angle, Vector3& axis) {

		glm_rotate(this->m, angle, axis.v);
		return *this;
	}

	Matrix4& Matrix4::Scale(Vector3& v) {
		glm_scale(this->m, v.v);
		return *this;
	}

	void Matrix4::Mult(const Matrix4& m1, const Matrix4& m2) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				m[i][j] = m1.m[i][0] * m2.m[0][j]
				+ m1.m[i][1] * m2.m[1][j]
				+ m1.m[i][2] * m2.m[2][j]
				+ m1.m[i][3] * m2.m[3][j];
	}
}
