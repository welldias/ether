#ifndef __ETHER_RGBA_H__
#define __ETHER_RGBA_H__

namespace ether {

	struct Rgba
	{
		float c[4];

		operator float*() const { return (float*)c; }

		Property<float> r;
		Property<float> g;
		Property<float> b;
		Property<float> a;

		Rgba();
		Rgba(float r, float g, float b, float a = 1);
		Rgba(const Rgba& c1);
		Rgba(const float* c1);
		Rgba(int hexColor);
		Rgba(const std::string& c1);

		inline float& operator[](int i) {
			if (i > 3) {
				throw MathError("Index out of bounds: Index: "+ std::to_string(i) +" Size: 4");
			}
			return c[i];
		}

		inline Rgba& operator= (const Rgba& c1) {

			c[0] = c1.c[0];
			c[1] = c1.c[1];
			c[2] = c1.c[2];
			c[3] = c1.c[3];
			return *this;
		}

		inline Rgba& operator= (int hexColor) {
			Set(hexColor);
			return *this;
		}

		inline Rgba& operator= (const std::string& webColor) {
			Set(webColor);
			return *this;
		}

		inline void  Zero() { c[0] = c[1] = c[2] = c[3] = 0; };
		inline void  Set(float r, float g, float b, float a = 1) { c[0] = r; c[1] = g;  c[2] = b; c[3] = a; };
		void  Set(const Rgba& c1);
		void  Set(int hexColor);
		void  Set(const std::string& webColor);
	};
}

#endif // __ETHER_RGBA_H__