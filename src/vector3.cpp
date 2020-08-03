#include "ether.h"

namespace ether {
	Vector3::Vector3()
		:x(v), y(v + 1), z(v + 2) {
		Zero();
	}
	
	Vector3::Vector3(float x, float y, float z)
		:x(v), y(v + 1), z(v + 2) {
		v[0] = x; v[1] = y;  v[2] = z;
	}

	Vector3::Vector3(const Vector3& v1)
		:x(v), y(v + 1), z(v + 2) {
		v[0] = v1.v[0]; v[1] = v1.v[1];  v[2] = v1.v[2];
	}

}
