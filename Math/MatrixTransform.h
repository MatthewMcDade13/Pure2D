#ifndef PURE2D_MATRIX_TRANSFORM_H
#define PURE2D_MATRIX_TRANSFORM_H

#include <Pure2D/Define.h>

namespace pure
{
	template<typename T> struct Vec3;
	struct Mat4;

	PURE2D_API Mat4 translate(const Mat4& mat, const Vec3<float>& vec);
	PURE2D_API Mat4 rotate(const Mat4& mat, float angle, const Vec3<float>& vec);
	PURE2D_API Mat4 scale(const Mat4& mat, const Vec3<float>& vec);
	PURE2D_API Mat4 inverse(const Mat4& mat);
	PURE2D_API Mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
	PURE2D_API Mat4 perspective(float fov, float aspect, float near, float far);
	PURE2D_API Mat4 lookAt(const Vec3<float>& position, const Vec3<float>& target, const Vec3<float>& up);
}

#endif // PURE2D_MATRIX_TRANSFORM_H



