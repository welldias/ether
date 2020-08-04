#include "ether.h"

namespace ether {
#define COLOR_I2F(i) ((1.0f / 255) * i)

	Rgba::Rgba()
		:r(c), g(c + 1), b(c + 2), a(c + 3) {
		Zero();
	}
	
	Rgba::Rgba(float r, float g, float b, float a)
		: r(c), g(c + 1), b(c + 2), a(c + 3) {
		Set(r, g, b, a);
	}

	Rgba::Rgba(const Rgba& c1)
		: r(c), g(c + 1), b(c + 2), a(c + 3) {
		Set(c1);
	}

	Rgba::Rgba(const float* c1) 
		: r(c), g(c + 1), b(c + 2), a(c + 3) {
		c[0] = c1[0]; c[1] = c1[1]; c[2] = c1[2]; c[3] = c1[3];
	}

	void Rgba::Set(const Rgba& c1) {
		c[0] = c1.c[0]; c[1] = c1.c[1]; c[2] = c1.c[2]; c[3] = c1.c[3];
	}

	Rgba::Rgba(int hexColor)
		: r(c), g(c + 1), b(c + 2), a(c + 3) {

		Set(hexColor);
	}

	Rgba::Rgba(const std::string& webColor)
		: r(c), g(c + 1), b(c + 2), a(c + 3) {
		Set(webColor);
	}

	void Rgba::Set(int hexColor) {

		int r = (hexColor & 0xFF000000) >> 24;
		int g = (hexColor & 0xFF0000) >> 16;
		int b = (hexColor & 0xFF00) >> 8;
		int a = (hexColor & 0xFF);

		if ((r < 0 && r > 255)
			|| (g < 0 && b > 255)
			|| (b < 0 && b > 255)
			|| (a < 0 && a > 255)) {
			throw ConversionError("Invalid hexColor value");
		}

		c[0] = COLOR_I2F(r);
		c[1] = COLOR_I2F(g);
		c[2] = COLOR_I2F(b);
		c[3] = COLOR_I2F(a);
	}

	void Rgba::Set(const std::string& webColor) {
		if (webColor.empty())
			Zero();
		
		std::string strColor = webColor.at(0) == '#' ? webColor.substr(1) : webColor;

		if(strColor.length() != 8 && strColor.length() != 6)
			throw ConversionError("Invalid webColor value");

		this->a = 1;

		int hexColor = 0;
		int x = 0;
		for (int i = 0; i < strColor.length(); i = i + 2) {
			std::istringstream(strColor.substr(i, 2)) >> std::hex >> hexColor;
			c[x++] = COLOR_I2F(hexColor);
		}
	}
}
