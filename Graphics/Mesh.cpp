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

//    const Vec2f texRectSize = { float(textureRect.w), float(textureRect.h) };
//    const Vec2f uvOffset = { float(textureRect.x), float(textureRect.y) };
//
//    for (size_t i = 0; i < Mesh::QUAD_VERT_COUNT; i++)
//    {
//        auto& v = verts[i];
//        v = quadVerts[i];
//
//        const Vec2f texDim = texRectSize * Vec2f(v.texCoord.u, 1.f - v.texCoord.v);
//        const Vec2f texCoord = texDim + uvOffset;
//        const Vec2f normalizedCoords = { texCoord.x / texture.size.x, 1.f - (texCoord.y / texture.size.y) };
//        v.texCoord = { normalizedCoords.x, normalizedCoords.y };
//    }

    Mesh m = Mesh::create(q.verts, Quad::VERT_COUNT, DrawPrimitive::TRIANGLES, usage);

    m.texture = &texture;
    return m;
}
