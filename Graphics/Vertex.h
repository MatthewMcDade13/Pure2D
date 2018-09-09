#ifndef PURE3D_VERTEX_H
#define PURE3D_VERTEX_H

#include <cinttypes>
#include "Define.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"


// TODO: Spread this out to renderer.

namespace pure
{

	struct Vertex
	{
		static constexpr uint32_t COUNT = 8;

		union
		{
			struct 
			{
				struct { float x, y, z; } position;
				struct { float x, y, z; } normal;
				struct { float x, y; } textureCoord;
			};

			float buffer[COUNT];
		};
	};

	struct Vertex2D
	{
		static constexpr uint32_t COUNT = 9;

		union
		{
			struct
			{
				struct { float x, y, z; } position;
				struct { float u, v; } texCoord;
				struct { float r, g, b, a; } color;
			};

			float buffer[COUNT];
		};
	};

}

#endif // PURE3D_VERTEX_H