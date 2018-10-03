//
// Created by matt on 10/3/18.
//

#include "Quad.h"
#include <algorithm>
#include <iterator>

using namespace pure;

static constexpr Vertex2D quadVerts[Quad::VERT_COUNT] = {
        { 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },
        { 0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   1.f, 1.f, 1.f, 1.f },

        { 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.f, 1.f, 1.f, 1.f },
        { 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.f, 1.f, 1.f, 1.f }
};

pure::Quad pure::Quad::create(pure::Vertex2D *verts)
{
    Quad q = {};
    if (verts == nullptr)
        std::copy(std::begin(quadVerts), std::end(quadVerts), q.verts);
    else
        std::copy(verts, verts + Quad::VERT_COUNT, q.verts);

    return q;

}
