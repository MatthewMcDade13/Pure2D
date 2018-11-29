#ifndef PURE2D_PRIVATE_CONVERT_H
#define PURE2D_PRIVATE_CONVERT_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Vec3.h>
#include <Pure2D/Math/Vec4.h>
#include <Pure2D/Math/Mat4.h>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Buffers.h>
#include <Pure2D/Window/Keyboard.h>
#include <SDL2/SDL_events.h>
#include "External/glad.h"

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

GLenum toGlUsage(pure::DrawUsage usage);

GLenum toGlPrim(pure::DrawPrimitive prim);
GLenum toGlBuffAccess(pure::BufferAccess access);

GLenum toGlDataType(pure::DataType dt);

GLenum toGlTexFormat(pure::Texture::Format fmt);

SDL_Scancode toSDLScancode(pure::Key key);
pure::Key toPureKey(SDL_Scancode code);


inline pure::Mat4 toMat4(const glm::mat4& mat) { return { *TO_ALIGNED_STORAGE(mat) }; }

#endif // PURE2D_PRIVATE_CONVERT_H