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
#if 0
		Matrix4  operator/ (float value) {
			if (value == 0) {
				throw MathError("Divid by zero");
			}
			return Matrix4(m[0] / value, m[1] / value, m[2] / value);
		}

		Matrix4& operator/=(float value) {
			if (value == 0) {
				throw MathError("Divid by zero");
			}

			m[0] /= value;
			m[1] /= value;
			m[2] /= value;
			return *this;
		}



		inline Matrix4  Negative() { 
			m[0] -= m[0]; 
			m[1] -= m[1];  
			m[2] -= m[2]; 
			return *this;
		};

		inline float Dotproduct(const Matrix4& m1) {
			return m[0] * m1.m[0] + m1.m[1] * m[1] + m1.m[2] * m[2];
		}

		inline Matrix4 Crossproduct(const Matrix4& m1) {
			float xf = m[1] * m1.m[2] - m[2] * m1.m[1];
			float yf = m[2] * m1.m[0] - m[0] * m1.m[2];
			float zf = m[0] * m1.m[1] - m[1] * m1.m[0];
			return Matrix4(xf, yf, zf);
		}

		inline float Magnitude() {
			float f = m[0] * m[0] + m[1] * m[1] + m[2] * m[2];
			return sqrt(f);
		}

		inline Matrix4 Normalize() {
			float f = Magnitude();
			if (f == 0) {
				Zero();
				return *this;
			}

			*this /= f;
			return *this;
		}

		inline float Distance(Matrix4& m1) {
			Matrix4 v2 = *this - m1;
			return v2.Magnitude();
		}

		inline Matrix4 Scale(float value) {
			m[0] *= value;
			m[1] *= value;
			m[2] *= value;

			return *this;
		}

		Matrix4 Rescale(float value) {
			Normalize();
			return Scale(value);
		}
#endif

		inline Matrix4& operator= (const Matrix4& m1) {
			memcpy(m, m1.m, sizeof(m));
			return *this;
		}

		void  Identity();
		inline void  Zero() { memset(m, 0, sizeof(m)); };

	private:
		inline void Mult(const Matrix4& m1, const Matrix4& m2) {
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					m[i][j] = m1.m[i][0] * m2.m[0][j] 
						+ m1.m[i][1] * m2.m[1][j] 
						+ m1.m[i][2] * m2.m[2][j]
						+ m1.m[i][3] * m2.m[3][j];
		}

	};
}

#endif // __ETHER_MATRIX4_H__