#include "ether.h"

namespace ether {

	const float MathUtil::PI = static_cast<float>(3.14159265359);
	const double MathUtil::SQRT3 = 1.7320508075688772935;
	const double MathUtil::SQRT5 = 2.2360679774997896964;

	void MathUtil::Transformation(Vector3 translation, Vector3 rotation, float scale, Matrix4& result) {

	}

	float MathUtil::Radian(float d) {
		return (d * PI / 180.0f);
	}

	float MathUtil::Degree(float r) {
		return (r * 180.0f / PI);
	}
}
