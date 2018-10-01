#ifndef PURE2D_MESH_H
#define PURE2D_MESH_H

#include <cinttypes>
#include "Graphics/Shader.h"
#include "Graphics/Buffers.h"

namespace pure
{
	struct Vertex2D;
	struct Texture;
	template<typename T> struct Rect;

	struct Mesh
	{
		VertexBuffer vbo;
		const Texture* texture;
		Shader shader;
		DrawPrimitive primtype;

		static Mesh create(const Vertex2D* verts, size_t vertCount, DrawPrimitive primtype = DrawPrimitive::TRIANGLES,
		        DrawUsage usage = DrawUsage::STATIC_DRAW);

		static Mesh quad(DrawUsage usage = DrawUsage::STATIC_DRAW);
		static Mesh quad(const Rect<uint32_t>& textureRect, Texture& texture, DrawUsage usage = DrawUsage::STATIC_DRAW);
	};
}

#endif // PURE2D_MESH_H
