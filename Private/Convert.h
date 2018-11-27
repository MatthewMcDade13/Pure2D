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

constexpr GLenum toGlUsage(pure::DrawUsage usage)
{
	using namespace pure;

	switch (usage)
	{
		case DrawUsage::STATIC_DRAW: return GL_STATIC_DRAW;
		case DrawUsage::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
		case DrawUsage::STREAM_DRAW: return GL_STREAM_DRAW;
	}

	return 0;
}

constexpr GLenum toGlPrim(pure::DrawPrimitive prim)
{
	using namespace pure;

	switch (prim)
	{
		case DrawPrimitive::POINTS: return GL_POINTS;
		case DrawPrimitive::LINES: return GL_LINES;
		case DrawPrimitive::LINE_LOOP: return GL_LINE_LOOP;
		case DrawPrimitive::LINE_STRIP: return GL_LINE_STRIP;
		case DrawPrimitive::TRIANGLES: return GL_TRIANGLES;
		case DrawPrimitive::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		case DrawPrimitive::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
		case DrawPrimitive::QUADS: return GL_QUADS;
		case DrawPrimitive::QUAD_STRIP: return GL_QUAD_STRIP;
		case DrawPrimitive::POLYGON: return GL_POLYGON;
	}

	return 0;
}

constexpr GLenum toGlBuffAccess(pure::BufferAccess access)
{
	using namespace pure;

	switch (access)
	{
		case BufferAccess::READ_ONLY: return GL_READ_ONLY;
		case BufferAccess::WRITE_ONLY: return GL_WRITE_ONLY;
		case BufferAccess::READ_WRITE: return GL_READ_WRITE;
	}
	return 0;
}

constexpr GLenum toGlDataType(pure::DataType dt)
{
	using namespace pure;

	switch (dt)
	{
		case DataType::BYTE: return GL_BYTE;
		case DataType::UBYTE: return GL_UNSIGNED_BYTE;
		case DataType::SHORT: return GL_SHORT;
		case DataType::USHORT: return GL_UNSIGNED_SHORT;
		case DataType::INT: return GL_INT;
		case DataType::UINT: return GL_UNSIGNED_INT;
		case DataType::FLOAT: return GL_FLOAT;
	}
	return 0;
}

constexpr GLenum toGlTexFormat(pure::Texture::Format fmt)
{
	using namespace pure;

	switch (fmt)
	{
		case Texture::Format::RED: return GL_RED;
		case Texture::Format::GREEN: return GL_GREEN;
		case Texture::Format::BLUE: return GL_BLUE;
		case Texture::Format::ALPHA: return GL_ALPHA;
		case Texture::Format::RGB: return GL_RGB;
		case Texture::Format::RGBA: return GL_RGBA;
		case Texture::Format::R8: return GL_R8;
	}

	return 0;
}

SDL_Scancode toSDLScancode(pure::Key key);
pure::Key toPureKey(SDL_Scancode code);


inline pure::Mat4 toMat4(const glm::mat4& mat) { return { *TO_ALIGNED_STORAGE(mat) }; }

#endif // PURE2D_PRIVATE_CONVERT_H