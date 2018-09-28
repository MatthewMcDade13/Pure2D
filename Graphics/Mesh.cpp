//
// Created by matt on 9/25/18.
//

#include "Mesh.h"
#include "Vertex.h"
#include "System/Util.h"

pure::Mesh pure::Mesh::quad(DrawUsage usage)
{
    Mesh m = {};

    constexpr Vertex2D verts[] = {
        { 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },
        { 0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },

        { 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f }
    };

    m.vbo = VertexBuffer::create(verts, PURE_ARRAY_COUNT(verts), usage);
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
