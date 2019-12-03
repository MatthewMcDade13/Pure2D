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

	// Wrapper type for Mesh data. Can assign optional Shader or Texture, owns its owns
	// internal vbo and ebo.
	struct PURE2D_API Mesh
	{

		VertexBuffer vbo;
		ElementBuffer ebo = { 0 };
		const Texture* texture;

		// TODO: Make this a pointer. Mesh ideally has optional Texture and/or Shader, but owns its internal
		// vbo and ebo (unless otherwise noted) doing this we can have Mesh comform to the free() api
		Shader shader;
		DrawPrimitive primtype;

		static Mesh make(const Vertex2D* verts, size_t vertCount, DrawPrimitive primtype = DrawPrimitive::TRIANGLES,
		        DrawUsage usage = DrawUsage::STATIC_DRAW);

		static Mesh quad(DrawUsage usage = DrawUsage::STATIC_DRAW);
		static Mesh quad(const Rect<uint32_t>& textureRect, Texture& texture, DrawUsage usage = DrawUsage::STATIC_DRAW);

	};
}

#endif // PURE2D_MESH_H
