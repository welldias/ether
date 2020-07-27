#ifndef __ETHER_MATH_UTIL_H__
#define __ETHER_MATH_UTIL_H__

namespace ether {

	class MathUtil
	{
	public:
		static const float PI;

	public:
		static void Transformation(Vector3 translation, Vector3 rotation, float scale, Matrix4& result);
		static float Radian(float d);
		static float Degree(float r);
	};
}

#endif // __ETHER_MATH_UTIL_H__