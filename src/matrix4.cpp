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

#if 0
		Matrix4 matAux(*this);

		matAux.Scale(v);

		for (int i = 0; i < 3; ++i)
			m[3][i] = matAux.m[3][i] + v[i];

		//vec4 v1, v2, v3;

		//glm_vec4_scale(m[0], v[0], v1);
		//glm_vec4_scale(m[1], v[1], v2);
		//glm_vec4_scale(m[2], v[2], v3);

		//glm_vec4_add(v1, m[3], m[3]);
		//glm_vec4_add(v2, m[3], m[3]);
		//glm_vec4_add(v3, m[3], m[3]);
#endif
		return *this;

	}

	Matrix4& Matrix4::Rotate(float angle, Vector3& axis) {

		glm_rotate(this->m, angle, axis.v);

#if 0
		Matrix4 rot;

		Vector3 axisn, v, vs;
		float c;

		c = cosf(angle);

		//glm_vec3_normalize_to(axis, axisn);
		//glm_vec3_scale(axisn, 1.0f - c, v);
		//glm_vec3_scale(axisn, sinf(angle), vs);
		axisn = axis;
		v = axisn.Normalize();
		v.Scale(1.0f - c);
		vs = axisn;
		vs.Scale(sinf(angle));
		//---------------------------------

		//glm_vec3_scale(axisn, v[0], rot.m[0]);
		//glm_vec3_scale(axisn, v[1], rot.m[1]);
		//glm_vec3_scale(axisn, v[2], rot.m[2]);
		Vector3 aux;
		axisn.Scale(v[0], aux);
		memcpy(rot.m[0], aux.v, sizeof(rot.m[0]));
		aux.Scale(v[1], aux);
		memcpy(rot.m[1], aux.v, sizeof(rot.m[1]));
		aux.Scale(v[2], aux);
		memcpy(rot.m[2], aux.v, sizeof(rot.m[2]));
		//---------------------------------

		rot.m[0][0] += c;       rot.m[1][0] -= vs[2];   rot.m[2][0] += vs[1];
		rot.m[0][1] += vs[2];   rot.m[1][1] += c;       rot.m[2][1] -= vs[0];
		rot.m[0][2] -= vs[1];   rot.m[1][2] += vs[0];   rot.m[2][2] += c;

		rot.m[0][3] = rot.m[1][3] = rot.m[2][3] = rot.m[3][0] = rot.m[3][1] = rot.m[3][2] = 0.0f;
		rot.m[3][3] = 1.0f;

		// chama glm_mul_rot(m, rot, m);
		//glm_mul_rot(mat4 m1, mat4 m2, mat4 dest) {

		//float a00 = m1[0][0], a01 = m1[0][1], a02 = m1[0][2], a03 = m1[0][3],
		//	a10 = m1[1][0], a11 = m1[1][1], a12 = m1[1][2], a13 = m1[1][3],
		//	a20 = m1[2][0], a21 = m1[2][1], a22 = m1[2][2], a23 = m1[2][3],
		//	a30 = m1[3][0], a31 = m1[3][1], a32 = m1[3][2], a33 = m1[3][3],
		Matrix4 matA;
		for (int i = 0; i < 4; ++i) 
			for (int j = 0; j < 4; ++j)
				matA.m[i][j] = m[i][j];
		//---------------------------------

		//b00 = m2[0][0], b01 = m2[0][1], b02 = m2[0][2],
		//b10 = m2[1][0], b11 = m2[1][1], b12 = m2[1][2],
		//b20 = m2[2][0], b21 = m2[2][1], b22 = m2[2][2];
		Matrix4 matB;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				matB.m[i][j] = rot.m[i][j];

		m[0][0] = matA.m[0][0] * matB.m[0][0] + matA.m[1][0] * matB.m[0][1] + matA.m[2][0] * matB.m[0][2];
		m[0][1] = matA.m[0][1] * matB.m[0][0] + matA.m[1][1] * matB.m[0][1] + matA.m[2][1] * matB.m[0][2];
		m[0][2] = matA.m[0][2] * matB.m[0][0] + matA.m[1][2] * matB.m[0][1] + matA.m[2][2] * matB.m[0][2];
		m[0][3] = matA.m[0][3] * matB.m[0][0] + matA.m[1][3] * matB.m[0][1] + matA.m[2][3] * matB.m[0][2];

		m[1][0] = matA.m[0][0] * matB.m[1][0] + matA.m[1][0] * matB.m[1][1] + matA.m[2][0] * matB.m[1][2];
		m[1][1] = matA.m[0][1] * matB.m[1][0] + matA.m[1][1] * matB.m[1][1] + matA.m[2][1] * matB.m[1][2];
		m[1][2] = matA.m[0][2] * matB.m[1][0] + matA.m[1][2] * matB.m[1][1] + matA.m[2][2] * matB.m[1][2];
		m[1][3] = matA.m[0][3] * matB.m[1][0] + matA.m[1][3] * matB.m[1][1] + matA.m[2][3] * matB.m[1][2];

		m[2][0] = matA.m[0][0] * matB.m[2][0] + matA.m[1][0] * matB.m[2][1] + matA.m[2][0] * matB.m[2][2];
		m[2][1] = matA.m[0][1] * matB.m[2][0] + matA.m[1][1] * matB.m[2][1] + matA.m[2][1] * matB.m[2][2];
		m[2][2] = matA.m[0][2] * matB.m[2][0] + matA.m[1][2] * matB.m[2][1] + matA.m[2][2] * matB.m[2][2];
		m[2][3] = matA.m[0][3] * matB.m[2][0] + matA.m[1][3] * matB.m[2][1] + matA.m[2][3] * matB.m[2][2];

		m[3][0] = matA.m[3][0];
		m[3][1] = matA.m[3][1];
		m[3][2] = matA.m[3][2];
		m[3][3] = matA.m[3][3];
#endif

		return *this;
	}

	Matrix4& Matrix4::Scale(Vector3& v) {

		glm_scale(this->m, v.v);

#if 0
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				m[i][j] = m[i][j] * v[i];

		/*
		m[0][0] = m[0][0] * v[0];
		m[0][1] = m[0][1] * v[0];
		m[0][2] = m[0][2] * v[0];
		m[0][3] = m[0][3] * v[0];

		m[1][0] = m[1][0] * v[1];
		m[1][1] = m[1][1] * v[1];
		m[1][2] = m[1][2] * v[1];
		m[1][3] = m[1][3] * v[1];

		m[2][0] = m[2][0] * v[2];
		m[2][1] = m[2][1] * v[2];
		m[2][2] = m[2][2] * v[2];
		m[2][3] = m[2][3] * v[2];
		*/
#endif
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
