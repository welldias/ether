#ifndef __ETHER_VECTOR3_H__
#define __ETHER_VECTOR3_H__

namespace ether {

	struct Vector3
	{
		float v[3];

		operator float*() const { return (float*)v; }

		Property<float> x;
		Property<float> y;
		Property<float> z;
		Property<float> r;
		Property<float> g;
		Property<float> b;

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3& v1);

		inline float& operator[](int i) {
			if (i > 2) {
				throw MathError("Index out of bounds: Index: "+ std::to_string(i) +" Size: 2");
			}
			return v[i];
		}

		inline Vector3 operator+ (const Vector3& v1) {
			return Vector3(v[0] + v1.v[0], v[1] + v1.v[1], v[2] + v1.v[2]);
		}

		inline Vector3& operator+=(const Vector3& v1) {
			v[0] += v1.v[0];
			v[1] += v1.v[1];
			v[2] += v1.v[2];
			return *this;
		}

		inline Vector3 operator- (const Vector3& v1) {
			return Vector3(v[0] - v1.v[0], v[1] - v1.v[1], v[2] - v1.v[2]);
		}

		inline Vector3& operator-=(const Vector3& v1) {
			v[0] -= v1.v[0];
			v[1] -= v1.v[1];
			v[2] -= v1.v[2];
			return *this;
		}

		inline Vector3  operator* (float value) {
			return Vector3(v[0] * value, v[1] * value, v[2] * value);
		}

		inline Vector3& operator*=(float value) {
			v[0] *= value;
			v[1] *= value;
			v[2] *= value;
			return *this;
		}

		inline Vector3  operator* (const Vector3& v1) {
			return Vector3(v[0] * v1.v[0], v[1] * v1.v[1], v[2] * v1.v[2]);
		}

		inline Vector3& operator*=(const Vector3& v1) {
			v[0] *= v1.v[0];
			v[1] *= v1.v[1];
			v[2] *= v1.v[2];
			return *this;
		}

		inline Vector3  operator/ (float value) {
			if (value == 0) {
				throw MathError("Divid by zero");
			}
			return Vector3(v[0] / value, v[1] / value, v[2] / value);
		}

		inline Vector3& operator/=(float value) {
			if (value == 0) {
				throw MathError("Divid by zero");
			}

			v[0] /= value;
			v[1] /= value;
			v[2] /= value;
			return *this;
		}

		inline Vector3& operator= (const Vector3& v1) {
			v[0] = v1.v[0];
			v[1] = v1.v[1];
			v[2] = v1.v[2];
			return *this;
		}

		inline void  Set(float x, float y, float z) { v[0] = x; v[1] = y;  v[2] = z; };
		inline void  Zero() { v[0] = v[1] = v[2] = 0; };
		
		inline Vector3  Negative() { 
			v[0] -= v[0]; 
			v[1] -= v[1];  
			v[2] -= v[2]; 
			return *this;
		};

		inline float Dotproduct(const Vector3& v1) {
			return v[0] * v1.v[0] + v1.v[1] * v[1] + v1.v[2] * v[2];
		}

		inline Vector3 Crossproduct(const Vector3& v1) {
			float xf = v[1] * v1.v[2] - v[2] * v1.v[1];
			float yf = v[2] * v1.v[0] - v[0] * v1.v[2];
			float zf = v[0] * v1.v[1] - v[1] * v1.v[0];
			return Vector3(xf, yf, zf);
		}

		inline float Magnitude() {
			float f = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
			return sqrt(f);
		}

		inline Vector3 Normalize() {
			float f = Magnitude();
			if (f == 0) {
				Zero();
				return *this;
			}

			*this /= f;
			return *this;
		}

		inline float Distance(Vector3& v1) {
			Vector3 v2 = *this - v1;
			return v2.Magnitude();
		}

		inline Vector3 Scale(float value) {
			v[0] *= value;
			v[1] *= value;
			v[2] *= value;

			return *this;
		}

		inline Vector3 Rescale(float value) {
			Normalize();
			return Scale(value);
		}
	};
}

#endif // __ETHER_VECTOR3_H__