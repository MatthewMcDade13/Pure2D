#include "MatrixTransform.h"
#include "Private/Convert.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"
#include <Pure2D/Math/Manip.h>
#include <Pure2D/Math/Constants.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace pure;

#if defined(_WIN32) || defined(_WIN64)
// Lmfao wtf is this shit. need to undef to avoid some meme macro smdh windows...
#undef near
#undef far
#endif

static inline glm::mat4 toglm(const Mat4& m)
{
	return glm::mat4(
		m.val.buf[0], m.val.buf[1], m.val.buf[2], m.val.buf[3],
		m.val.buf[4], m.val.buf[5], m.val.buf[6], m.val.buf[7],
		m.val.buf[8], m.val.buf[9], m.val.buf[10], m.val.buf[11],
		m.val.buf[12], m.val.buf[13], m.val.buf[14], m.val.buf[15]
	);
}

// TODO: Remove dependency on GLM

Mat4 pure::translate(const Mat4 & mat, const Vec3<float>& vec)
{
	Mat4 m = mat;
	m.val.col[3] += Vec4f(vec, 0.f);
	return m;
	//return toMat4(glm::translate(toglm(mat), toGlmVec3(vec)));
}

Mat4 pure::rotate(const Mat4 & mat, float angle, const Vec3<float>& vec)
{
	return toMat4(glm::rotate(toglm(mat), angle, toGlmVec3(vec)));
}

Mat4 pure::rotate2D(const Mat4 & mat, float angleDeg)
{
	const float angle = angleDeg * DEG_TO_RAD;
	const float c = cosf(angle);
	const float s = sinf(angle);

	Mat4 m;
	m.val.col[0] = { c, s, 0.f, 0.f };
	m.val.col[1] = { -s, c, 0.f, 0.f };
	m.val.col[2] = { 0.f, 0.f, 1.f, 0.f };
	m.val.col[3] = { 0.f, 0.f, 0.f, 1.f };

	return mat * m;
}

Mat4 pure::scale(const Mat4 & mat, const Vec3<float>& vec)
{
	Mat4 m = mat;
	m.val.col[0] *= vec.x;
	m.val.col[1] *= vec.y;
	m.val.col[2] *= vec.z;
	return m;
}

Mat4 pure::orthographic(float left, float right, float bottom, float top, float near, float far)
{
	return toMat4(glm::ortho(left, right, bottom, top, near, far));
}

Mat4 pure::perspective(float fov, float aspect, float near, float far)
{
	return toMat4(glm::perspective(fov, aspect, near, far));
}

Mat4 pure::lookAt(const Vec3<float>& position, const Vec3<float>& target, const Vec3<float>& up)
{
	return toMat4(glm::lookAt(toGlmVec3(position), toGlmVec3(target), toGlmVec3(up)));
}

Mat4 pure::inverse(const Mat4 &mat)
{
	return toMat4(glm::inverse(*TO_GLM_MAT4_CONST(mat)));
}