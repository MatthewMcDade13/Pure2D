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

    m.primtype = DrawPrimitive::TRIANGLE_STRIP;
    Quad q = Quad::create();
    m.vbo = VertexBuffer::create(q.verts, Quad::VERT_COUNT, usage);
    return m;

}

pure::Mesh pure::Mesh::create(const pure::Vertex2D *verts, size_t vertCount,
        pure::DrawPrimitive primtype, pure::DrawUsage usage)
{
    Mesh m = {};

    m.primtype = primtype;
    m.vbo = VertexBuffer::create(verts, vertCount, usage);
    return m;
}

pure::Mesh pure::Mesh::quad(const pure::Rectui &textureRect, Texture& texture, pure::DrawUsage usage)
{
    Quad q = Quad::create();

    calcTexCoords(q.verts, Quad::VERT_COUNT, textureRect, texture.size);

    Mesh m = Mesh::create(q.verts, Quad::VERT_COUNT, DrawPrimitive::TRIANGLE_STRIP, usage);

    m.texture = &texture;
    return m;
}
