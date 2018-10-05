#ifndef PURE2D_VERTEX_H
#define PURE2D_VERTEX_H

#include <cinttypes>
#include <cstddef>
#include "Define.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"

namespace pure
{
    struct Mat4;
    template<typename T> struct Rect;

    struct Vertex2D
    {
        static constexpr uint32_t COUNT = 9;

        union
        {
            struct
            {
                struct { float x, y, z; } position;
                struct { float u, v; } texCoord;
                struct { float r, g, b, a; } color;
            };

            float buffer[COUNT];
        };
    };

    void translateVerts(Vertex2D* verts, size_t vertCount, const Mat4& modelMat);
    // TODO: Probably don't need vertCount, might be safe to assume this is Quad::VERT_COUNT (since we are uing a textureRect
    // to find coordinates)
    void calcTexCoords(Vertex2D* verts, size_t vertCount, const Rect<uint32_t>& textureRect, Vec2i textureSize);
}

#endif // PURE2D_VERTEX_H