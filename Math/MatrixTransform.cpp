#include "MatrixTransform.h"
#include "Private/Convert.h"
#include "Math/Mat4.h"
#include "Math/Vec3.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace pure;

#if defined(_WIN32) || defined(_WIN64)
// Lmfao wtf is this shit. need to undef to avoid some meme macro smdh windows...
#undef near
#undef far
#endif

// TODO: Remove dependency on GLM

Mat4 pure::translate(const Mat4 & mat, const Vec3<float>& vec)
{
	return toMat4(glm::translate(*TO_GLM_MAT4_CONST(mat.val_), toGlmVec3(vec)));
}

Mat4 pure::rotate(const Mat4 & mat, float angle, const Vec3<float>& vec)
{
	return toMat4(glm::rotate(GLM_MAT4_CONST(mat), angle, toGlmVec3(vec)));
}

Mat4 pure::scale(const Mat4 & mat, const Vec3<float>& vec)
{
	return toMat4(glm::scale(GLM_MAT4_CONST(mat), toGlmVec3(vec)));
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