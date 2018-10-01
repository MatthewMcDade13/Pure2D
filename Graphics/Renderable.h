//
// Created by matt on 9/28/18.
//

#ifndef PURE2D_RENDERABLE_H
#define PURE2D_RENDERABLE_H

#include "Define.h"

namespace pure
{
    struct PURE2D_API Renderable
    {
        virtual ~Renderable() = default;
        virtual void draw(struct Renderer& renderer) = 0;
    };
}

#endif //PURE2D_RENDERABLE_H
