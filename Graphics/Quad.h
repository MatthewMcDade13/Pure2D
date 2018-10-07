//
// Created by matt on 10/3/18.
//

#ifndef PURE2D_QUAD_H
#define PURE2D_QUAD_H

#include "Graphics/Vertex.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Graphics/Buffers.h"


namespace pure
{
    template <typename T> struct Rect;
    struct Mesh;


    struct Quad
    {
        static constexpr int VERT_COUNT = 4;
        Vertex2D verts[VERT_COUNT];

        static Quad create(Vertex2D* verts = nullptr);
        static Quad create(Vec3f pos, Vec2f size, float rotation = 0, const Rect<uint32_t>* textureRect = nullptr, Vec2i texSize = {});
        static Quad create(const Mat4& transform, const Rect<uint32_t>* textureRect = nullptr, Vec2i texSize = {});

        Mesh toMesh(DrawUsage usage = DrawUsage::STATIC_DRAW) const;

        void setTextureCoords(const Rect<uint32_t>& textureRect, Vec2i textureSize);
        void translate(const Mat4& transform);
    };
}


#endif //PURE2D_QUAD_H
