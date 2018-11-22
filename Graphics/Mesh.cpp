//
// Created by matt on 9/25/18.
//

#include <algorithm>
#include <iterator>
#include "Mesh.h"
#include "Vertex.h"
#include "System/Util.h"
#include "Graphics/Texture.h"
#include "Graphics/Quad.h"
#include "Math/Rect.h"

using namespace pure;

pure::Mesh pure::Mesh::quad(DrawUsage usage)
{
    Mesh m = {};

    m.primtype = DrawPrimitive::TRIANGLES;
    Quad q = Quad::make();
    m.vbo = VertexBuffer::make(q.verts, Quad::VERT_COUNT, usage);
	m.ebo = ElementBuffer::quad(6);
    return m;

}

pure::Mesh pure::Mesh::make(const pure::Vertex2D *verts, size_t vertCount,
        pure::DrawPrimitive primtype, pure::DrawUsage usage)
{
    Mesh m = {};

    m.primtype = primtype;
    m.vbo = VertexBuffer::make(verts, vertCount, usage);
    return m;
}

pure::Mesh pure::Mesh::quad(const pure::Rectui &textureRect, Texture& texture, pure::DrawUsage usage)
{
    Quad q = Quad::make();

    calcTexCoords(q.verts, Quad::VERT_COUNT, textureRect, texture.size);

    Mesh m = Mesh::make(q.verts, Quad::VERT_COUNT, DrawPrimitive::TRIANGLE_STRIP, usage);

    m.texture = &texture;
    return m;
}
