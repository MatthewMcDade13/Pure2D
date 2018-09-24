#ifndef PURE2D_MESH_H
#define PURE2D_MESH_H

#include <cinttypes>
#include "Graphics/Shader.h"

namespace pure
{
	struct Vertex2D;
	struct Texture;

	struct Mesh
	{
		const Vertex2D* verts;
		size_t numVerts;
		const Texture* texture;
		Shader shader;
	};
}

#endif // PURE2D_MESH_H
