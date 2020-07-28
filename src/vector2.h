#ifndef __ETHER_VECTOR2_H__
#define __ETHER_VECTOR2_H__

namespace ether {

	struct Vector2
	{
		float v[2];

		operator float*() const { return (float*)v; }

		Property<float> x;
		Property<float> y;

		Vector2();
		Vector2(float x, float y);
		Vector2(const Vector2& v1);

		inline float& operator[](int i) {
			if (i > 1) {
				throw MathError("Index out of bounds: Index: "+ std::to_string(i) +" Size: 2");
			}
			return v[i];
		}

		inline Vector2 operator+ (const Vector2& v1) {
			return Vector2(v[0] + v1.v[0], v[1] + v1.v[1]);
		}

		inline Vector2& operator+=(const Vector2& v1) {
			v[0] += v1.v[0];
			v[1] += v1.v[1];
			return *this;
		}

		inline Vector2 operator- (const Vector2& v1) {
			return Vector2(v[0] - v1.v[0], v[1] - v1.v[1]);
		}

		inline Vector2& operator-=(const Vector2& v1) {
			v[0] -= v1.v[0];
			v[1] -= v1.v[1];
			return *this;
		}

		inline Vector2  operator* (float value) {
			return Vector2(v[0] * value, v[1] * value);
		}

		inline Vector2& operator*=(float value) {
			v[0] *= value;
			v[1] *= value;
			return *this;
		}

		inline Vector2  operator* (const Vector2& v1) {
			return Vector2(v[0] * v1.v[0], v[1] * v1.v[1]);
		}

		inline Vector2& operator*=(const Vector2& v1) {
			v[0] *= v1.v[0];
			v[1] *= v1.v[1];
			return *this;
		}

		inline Vector2  operator/ (float value) {
			if (value == 0) {
				throw MathError("Divid by zero");
			}
			return Vector2(v[0] / value, v[1] / value);
		}

		inline Vector2& operator/=(float value) {
			if (value == 0) {
				throw MathError("Divid by zero");
			}

			v[0] /= value;
			v[1] /= value;
			return *this;
		}

		inline Vector2& operator= (const Vector2& v1) {
			v[0] = v1.v[0];
			v[1] = v1.v[1];
			return *this;
		}

		inline void  Set(float x, float y) { v[0] = x; v[1] = y;};
		inline void  Zero() { v[0] = v[1] = 0; };
		
		inline Vector2  Negative() { 
			v[0] -= v[0]; 
			v[1] -= v[1];  
			return *this;
		};

		inline float Dotproduct(Vector2& v1) {
			return glm_vec2_dot(v, v1.v);
		}

		inline float Crossproduct(Vector2& v1) {
			return glm_vec2_cross(v, v1.v);
		}

		inline Vector2 Normalize() {
			glm_vec2_normalize(v);
			return *this;
		}

		inline float Distance(Vector2& v1) {
			return glm_vec2_distance(v, v1.v);
		}

		inline Vector2 Scale(float value) {
			v[0] *= value;
			v[1] *= value;
			return *this;
		}

		inline Vector2 Scale(float value, Vector2& dest) {
			dest[0] = v[0] * value;
			dest[1] = v[1] * value;
			return dest;
		}
	};
}

#endif // __ETHER_VECTOR2_H__