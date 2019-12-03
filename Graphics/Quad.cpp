//
// Created by matt on 10/3/18.
//

#include "Graphics/Quad.h"
#include "Graphics/Transform.h"
#include "Graphics/Mesh.h"
#include "Math/Mat4.h"
#include "Quad.h"

#include <algorithm>
#include <iterator>

using namespace pure;

static inline void resetTexCoords(Quad& q);
static inline void resetPositions(Quad& q);

static constexpr Vertex2D quadVerts[Quad::VERT_COUNT] = {
    { 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },  // top left
    { 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },  // bottom left
    { 1.f, 0.f, 0.0f,    1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },  // top right
    { 1.f, 1.f, 0.0f,    1.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },  // bottom right
};

pure::Quad pure::Quad::make(pure::Vertex2D *verts)
{
    Quad q = {};
    if (verts == nullptr)
        std::copy(std::begin(quadVerts), std::end(quadVerts), q.verts);
    else
        std::copy(verts, verts + Quad::VERT_COUNT, q.verts);

    return q;

}

Quad Quad::make(Vec3f pos, Vec2f size, float rotation, const Rect<uint32_t> *textureRect, Vec2i texSize)
{

    Transform transform = Transform::make();
    transform.setPosition(pos);
    transform.setSize(size);
    transform.setRotation(rotation);

    return Quad::make(transform.modelMatrix(), textureRect, texSize);

}

Quad Quad::make(const Mat4& transform, const Rect<uint32_t> *textureRect, Vec2i texSize)
{
    Quad q = Quad::make();

    translateVerts(q.verts, Quad::VERT_COUNT, transform);

    if (textureRect)
        calcTexCoords(q.verts, Quad::VERT_COUNT, *textureRect, texSize);

    return q;
}

Mesh Quad::toMesh(DrawUsage usage) const
{
    return Mesh::make(verts, Quad::VERT_COUNT, DrawPrimitive::TRIANGLE_STRIP, usage);
}

void pure::Quad::flipVerticalTexCoords(float min, float max)
{
	pure::flipVerticalTexCoords(verts, Quad::VERT_COUNT, min, max);
}

void Quad::setTextureCoords(const Rect<uint32_t> &textureRect, Vec2i textureSize)
{
    resetTexCoords(*this);
    calcTexCoords(verts, Quad::VERT_COUNT, textureRect, textureSize);
}

void Quad::translate(const Mat4 &transform)
{
    resetPositions(*this);
    translateVerts(verts, Quad::VERT_COUNT, transform);
}

void resetTexCoords(Quad& q)
{
    for (int i = 0; i < Quad::VERT_COUNT; i++)
        q.verts[i].texCoord = quadVerts[i].texCoord;
}

void resetPositions(Quad& q)
{
    for (int i = 0; i < Quad::VERT_COUNT; i++)
        q.verts[i].position = quadVerts[i].position;
}