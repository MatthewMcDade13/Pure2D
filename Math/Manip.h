#ifndef PURE2D_MATH_MANIP_H
#define PURE2D_MATH_MANIP_H

#include <Pure2D/Define.h>

namespace pure
{
	template<typename T> struct Vec2;
	template<typename T> struct Vec3;
	template<typename T> struct Vec4;

	PURE2D_API Vec2<float> normalize(const Vec2<float>& vec);
	PURE2D_API Vec3<float> normalize(const Vec3<float>& vec);
	PURE2D_API Vec4<float> normalize(const Vec4<float>& vec);

	PURE2D_API Vec3<float> cross(const Vec3<float>& lvec, const Vec3<float>& rvec);

	PURE2D_API float dot(const Vec2<float>& lvec, const Vec2<float>& rvec);
	PURE2D_API float dot(const Vec3<float>& lvec, const Vec3<float>& rvec);
	PURE2D_API float dot(const Vec4<float>& lvec, const Vec4<float>& rvec);

	PURE2D_API float clamp(float a, float min, float max);
}

#endif
