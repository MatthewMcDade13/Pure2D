#ifndef PURE2D_MESH_H
#define PURE2D_MESH_H

#include <cinttypes>
#include "Graphics/Shader.h"
#include "Graphics/Buffers.h"

namespace pure
{
	struct Vertex2D;
	struct Texture;

	struct Mesh
	{
		VertexBuffer vbo;
		const Texture* texture;
		Shader shader;

		static Mesh create(const Vertex2D* verts, size_t vertCount, DrawUsage usage = DrawUsage::STATIC_DRAW);
		static Mesh quad(DrawUsage usage = DrawUsage::STATIC_DRAW);
	};
}

#endif // PURE2D_MESH_H
