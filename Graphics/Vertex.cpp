//
// Created by matt on 10/3/18.
//

#include "Vertex.h"
#include "Graphics/Vertex.h"
#include "Math/Rect.h"
#include "Math/Mat4.h"

using namespace pure;

void pure::translateVerts(Vertex2D *verts, size_t vertCount, const Mat4 &transform)
{
    for (size_t i = 0; i < vertCount; i++)
    {
        auto& v = verts[i];
        const Vec4f pos = { v.position.x, v.position.y, v.position.z, 1.f };
        const Vec4f result = transform * pos;
        v.position = { result.x, result.y, result.z };
    }
}

void pure::calcTexCoords(Vertex2D* verts, size_t vertCount, const Rectui& textureRect, Vec2i textureSize)
{
    const Vec2f texRectSize = { float(textureRect.w), float(textureRect.h) };
    const Vec2f uvOffset = { float(textureRect.x), float(textureRect.y) };

    for (size_t i = 0; i < vertCount; i++)
    {
        auto& v = verts[i];

        const Vec2f texDim = texRectSize * Vec2f(v.texCoord.u, 1.f - v.texCoord.v);
        const Vec2f texCoord = texDim + uvOffset;
        const Vec2f normalizedCoords = { texCoord.x / textureSize.x, 1.f - (texCoord.y / textureSize.y) };
        v.texCoord = { normalizedCoords.x, normalizedCoords.y };
    }

}