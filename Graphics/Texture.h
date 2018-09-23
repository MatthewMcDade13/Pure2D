#ifndef PURE2D_TEXTURE_H
#define PURE2D_TEXTURE_H

#include <cinttypes>
#include "Define.h"
#include "Math/Vec2.h"

namespace pure
{

	enum class TextureFormat
	{
		RED = 0x1903,
		GREEN = 0x1904,
		BLUE = 0x1905,
		ALPHA = 0x1906,
		RGB = 0x1907,
		RGBA = 0x1908
	};

	struct Texture
	{
		Vec2i size;
		uint32_t id_;

		PURE2D_API uint32_t unit() const;
		PURE2D_API void setUnit(uint32_t unitNum);

		PURE2D_API void bind() const;

		PURE2D_API static Texture create(const char* fileName, bool shouldFlip = true);
		PURE2D_API static void free(Texture& tex);
	private:
		uint32_t m_unit;
	};

}

#endif // PURE2D_TEXTURE_H



