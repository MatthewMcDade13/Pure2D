//
// Created by matt on 10/3/18.
//

#ifndef PURE2D_QUAD_H
#define PURE2D_QUAD_H

#include "Vertex.h"

namespace pure
{
    struct Quad
    {
        static constexpr int VERT_COUNT = 4;
        static Quad create(Vertex2D* verts = nullptr);
        Vertex2D verts[VERT_COUNT];
    };
}


#endif //PURE2D_QUAD_H
