#include "ether.h"

namespace ether {
	Vector2::Vector2()
		:x(v), y(v + 1) {
		Zero();
	}
	
	Vector2::Vector2(float x, float y)
		:x(v), y(v + 1) {
		v[0] = x; v[1] = y;
	}

	Vector2::Vector2(const Vector2& v1)
		:x(v), y(v + 1) {
		v[0] = v1.v[0]; v[1] = v1.v[1];
	}
}
