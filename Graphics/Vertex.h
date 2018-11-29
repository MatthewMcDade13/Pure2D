#ifndef PURE2D_VERTEX_H
#define PURE2D_VERTEX_H

#include <cinttypes>
#include <cstddef>
#include <Pure2D/Define.h>
#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Vec3.h>
#include <Pure2D/Math/Vec4.h>

namespace pure
{
    struct Mat4;
    template<typename T> struct Rect;

    struct Vertex2D
    {
        static constexpr uint32_t COUNT = 9;

        struct Position 
        { 
            float x, y, z;
            static constexpr Position fromVec3(const Vec3f& pos) { return { pos.x, pos.y, pos.z }; }
            inline Vec3f toVec3() { return Vec3f(x, y, z); }
        };

        struct TextureCoordinate 
        { 
            float u, v;
            static constexpr TextureCoordinate fromVec2(Vec2f texel) { return { texel.x, texel.y }; }
            inline Vec2f toVec2() { return Vec2f(u, v); }
        };

        struct Color 
        { 
            float r, g, b, a;
            static constexpr Color fromVec4(const Vec4& color) { return { color.r, color.g, color.b, color.a }; }
            inline Vec4f toVec4() { return Vec4f(r, g, b, a); }
        };

        union
        {
            struct
            {
                Position position;
                TextureCoordinate texCoord;
                Color color;
            };

            float buffer[COUNT];
        };
    };

    void setColor(Vertex2D* verts, size_t vertCount, const Vec4f& color);

    void translateVerts(Vertex2D* verts, size_t vertCount, const Mat4& transform);
    // TODO: Probably don't need vertCount, might be safe to assume this is Quad::VERT_COUNT (since we are uing a textureRect
    // to find coordinates)
    void calcTexCoords(Vertex2D* verts, size_t vertCount, const Rect<uint32_t>& textureRect, Vec2i textureSize);

	// Flips given verts tex coordinates along vertical (y) axis. Assumes
	// coordinates are NDC
	void flipVerticalTexCoords(Vertex2D* verts, size_t count, float min, float max);
}

#endif // PURE2D_VERTEX_H