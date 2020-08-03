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

		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3& v1);

		inline float& operator[](int i) {
			if (i > 2) {
				throw MathError("Index out of bounds: Index: "+ std::to_string(i) +" Size: 3");
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
		inline void  SetXup() { v[0] = 1.0f; v[1] = 0.0f;  v[2] = 0.0f; };
		inline void  SetYup() { v[0] = 0.0f; v[1] = 1.0f;  v[2] = 0.0f; };
		inline void  SetZup() { v[0] = 0.0f; v[1] = 0.0f;  v[2] = 1.0f; };
		inline void  SetForward() { v[0] = 0.0f; v[1] = 0.0f;  v[2] = -1.0f; };

		
		inline Vector3  Negative() { 
			v[0] -= v[0]; 
			v[1] -= v[1];  
			v[2] -= v[2]; 
			return *this;
		};

		inline float Dotproduct(Vector3& v1) {
			return glm_vec3_dot(v, v1.v);
			//return v[0] * v1.v[0] + v1.v[1] * v[1] + v1.v[2] * v[2];
		}

		inline Vector3 Crossproduct(Vector3& v1) {
			Vector3 result;

			glm_vec3_cross(v, v1.v, result.v);
			/*
			float xf = v[1] * v1.v[2] - v[2] * v1.v[1];
			float yf = v[2] * v1.v[0] - v[0] * v1.v[2];
			float zf = v[0] * v1.v[1] - v[1] * v1.v[0];
			return Vector3(xf, yf, zf);
			*/
			return result;
		}

		inline float Magnitude() {
			return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		}

		inline Vector3 Normalize() {
			glm_vec3_normalize(v);

			/*
			float f = Magnitude();
			if (f == 0) {
				v[0] = v[1] = v[2] = 0.0f;
				return *this;
			}

			v[0] *= (1.0f / f);
			v[1] *= (1.0f / f);
			v[2] *= (1.0f / f);
			*/

			return *this;
		}

		inline float Distance(Vector3& v1) {
			return glm_vec3_distance(v, v1.v);
			/*
			Vector3 v2 = *this - v1;
			return v2.Magnitude();
			*/
		}

		inline Vector3 Scale(float value) {

			v[0] *= value;
			v[1] *= value;
			v[2] *= value;

			return *this;
		}

		inline Vector3 Scale(float value, Vector3& dest) {

			dest[0] = v[0] * value;
			dest[1] = v[1] * value;
			dest[2] = v[2] * value;

			return dest;
		}

		inline Vector3 Rescale(float value) {
			Normalize();
			return Scale(value);
		}
	};
}

#endif // __ETHER_VECTOR3_H__