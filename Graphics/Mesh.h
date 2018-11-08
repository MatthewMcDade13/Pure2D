#ifndef PURE2D_MESH_H
#define PURE2D_MESH_H

#include <Pure2D/Define.h>
#include <cinttypes>
#include <Pure2D/Graphics/Shader.h>
#include <Pure2D/Graphics/Buffers.h>

namespace pure
{
	struct Vertex2D;
	struct Texture;
	struct Mat4;
	template<typename T> struct Rect;

	// TODO: Let this have an optional ElementBuffer
	struct PURE2D_API Mesh
	{

		VertexBuffer vbo;
		ElementBuffer ebo = { 0 };
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
