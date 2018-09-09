#ifndef PURE3D_PRIVATE_CONVERT_H
#define PURE3D_PRIVATE_CONVERT_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Math/Mat4.h"

#define TO_ALIGNED_STORAGE(glm_mat4) reinterpret_cast<const std::aligned_storage_t<sizeof(float[4][4])>*>(&glm_mat4)

#define TO_GLM_MAT4(aligned_storage) reinterpret_cast<glm::mat4*>(&aligned_storage)
#define TO_GLM_MAT4_CONST(const_aligned_storage) reinterpret_cast<const glm::mat4*>(&const_aligned_storage)
#define GLM_MAT4(x) *TO_GLM_MAT4(x.val_)
#define GLM_MAT4_CONST(x) *TO_GLM_MAT4_CONST(x.val_)

template<typename Vec2>
inline glm::vec2 toGlmVec2(const Vec2& vec)
{
	return {
		vec.x, vec.y
	};
}


template<typename Vec3>
inline glm::vec3 toGlmVec3(const Vec3& vec)
{
	return {
		vec.x, vec.y, vec.z
	};
}

template<typename Vec4>
inline glm::vec4 toGlmVec4(const Vec4& vec)
{
	return {
		vec.x, vec.y, vec.z, vec.w
	};
}

constexpr pure::Vec4f toVec4(const glm::vec4& vec)
{
	return {
		vec.x, vec.y, vec.z, vec.w
	};
}

constexpr pure::Vec3f toVec3(const glm::vec3& vec)
{
	return {
		vec.x, vec.y, vec.z
	};
}

constexpr pure::Vec2f toVec2(const glm::vec2& vec)
{
	return {
		vec.x, vec.y
	};
}



inline pure::Mat4 toMat4(const glm::mat4& mat) { return { *TO_ALIGNED_STORAGE(mat) }; }

#endif // PURE3D_PRIVATE_CONVERT_H