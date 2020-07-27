#ifndef __ETHER_MATRIX4_H__
#define __ETHER_MATRIX4_H__

namespace ether {

	// Why shouldn’t Matrix look like an array - of - array ?
	// There is a good reason for this at the following link:
	// https://isocpp.org/wiki/faq/operator-overloading

	struct Matrix4
	{
		float m[4][4];

		operator float*() const { return (float*)m; }

		Matrix4() {};
		Matrix4(const Matrix4& m1);

		inline float& operator() (unsigned i, unsigned j) {
			if (i >= 4 || j >= 4)
				throw MathError("Index out of bounds: Index: row:" + std::to_string(i) + " col:" + std::to_string(j));
			return m[i][j];
		}

		inline Matrix4 operator+(const Matrix4& m1) {
			Matrix4 result;
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					result.m[i][j] = m[i][j] + m1.m[i][j];
			return result;
		}

		Matrix4& operator+=(const Matrix4& m1) {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m[i][j] += m1.m[i][j];
			return *this;
		}

		Matrix4  operator- (const Matrix4& m1) {
			Matrix4 result;
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					result.m[i][j] = m[i][j] - m1.m[i][j];
			return result;
		}

		Matrix4& operator-=(const Matrix4& m1) {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
 					m[i][j] -= m1.m[i][j];
			return *this;
		}

		Matrix4  operator* (float value) {
			Matrix4 result;
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					result.m[i][j] = m[i][j] * value;
			return result;
		}

		Matrix4& operator*=(float value) {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m[i][j] *= value;
			return *this;
		}

		Matrix4  operator* (const Matrix4& m1) {
			Matrix4 result;
			result.Mult(*this, m1);
			return result;
		}

		Matrix4& operator*=(const Matrix4& m1) {
			Matrix4 result;
			result.Mult(*this, m1);
			memcpy(m, result.m, sizeof(m));
			return *this;
		}

		Matrix4  operator* (Vector3& v1) {
			Matrix4 result(*this);
			return result.Scale(v1);
		}

		Matrix4& operator*=(Vector3& v1) {
			Scale(v1);
			return *this;
		}

		inline Matrix4& operator= (const Matrix4& m1) {
			memcpy(m, m1.m, sizeof(m));
			return *this;
		}

		Matrix4& Translate(Vector3& v);
		Matrix4& Rotate(float angle, Vector3& axis);
		Matrix4& Scale(Vector3& v);

		void Identity();
		inline void Zero() { memset(m, 0, sizeof(m)); };

	private:
		void Mult(const Matrix4& m1, const Matrix4& m2);

	};
}

#endif // __ETHER_MATRIX4_H__