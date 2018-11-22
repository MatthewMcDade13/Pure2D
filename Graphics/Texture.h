#ifndef PURE2D_TEXTURE_H
#define PURE2D_TEXTURE_H

#include <cinttypes>
#include <Pure2D/Define.h>
#include <Pure2D/Math/Vec2.h>
#include <Pure2D/System/DataType.h>

namespace pure
{


	struct Texture
	{
		enum class Format
		{
			RED = 0x1903,
			GREEN = 0x1904,
			BLUE = 0x1905,
			ALPHA = 0x1906,
			RGB = 0x1907,
			RGBA = 0x1908,
			R8 = 0x8229
		};



		Vec2i size;
		uint32_t id_;

		PURE2D_API uint32_t unit() const;
		PURE2D_API void setUnit(uint32_t unitNum);

		PURE2D_API void bind() const;

		PURE2D_API static void setAlignment(int alignment);
		PURE2D_API static Texture make(const char* fileName, bool shouldFlip = true);
		PURE2D_API static Texture make(size_t w, size_t h, 
			Format internalFormat, Format format, void* data, DataType type = DataType::UBYTE);
		PURE2D_API static Texture createBlank();
		PURE2D_API void free();

		PURE2D_API void write(Vec2i offset, size_t w, size_t h, Format format, DataType type, void* data);
	private:
		uint32_t m_unit;
	};

}

#endif // PURE2D_TEXTURE_H



