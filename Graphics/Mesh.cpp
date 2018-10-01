//
// Created by matt on 9/25/18.
//

#include "Mesh.h"
#include "Vertex.h"
#include "System/Util.h"
#include "Graphics/Texture.h"
#include "Math/Rect.h"

using namespace pure;

static constexpr Vertex2D quadVerts[] = {
        { 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },
        { 0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },

        { 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f }
};
static constexpr size_t quadVertsCount = PURE_ARRAY_COUNT(quadVerts);


pure::Mesh pure::Mesh::quad(DrawUsage usage)
{
    Mesh m = {};

    m.primtype = DrawPrimitive::TRIANGLES;
    m.vbo = VertexBuffer::create(quadVerts, PURE_ARRAY_COUNT(quadVerts), usage);
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
    Vertex2D verts[quadVertsCount] = {};

    const Vec2f texRectSize = { float(textureRect.w), float(textureRect.h) };
    const Vec2f uvOffset = { float(textureRect.x), float(textureRect.y) };

    for (size_t i = 0; i < quadVertsCount; i++)
    {
        auto& v = verts[i];
        v = quadVerts[i];

        const Vec2f texDim = texRectSize * Vec2f(v.texCoord.u, 1.f - v.texCoord.v);
        const Vec2f texCoord = texDim + uvOffset;
        const Vec2f normalizedCoords = { texCoord.x / texture.size.x, 1.f - (texCoord.y / texture.size.y) };
        v.texCoord = { normalizedCoords.x, normalizedCoords.y };
    }

    Mesh m = Mesh::create(verts, quadVertsCount, DrawPrimitive::TRIANGLES, usage);

    m.texture = &texture;
    return m;
}

