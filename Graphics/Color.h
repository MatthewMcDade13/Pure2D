#ifndef PURE2D_GRAPHICS_COLOR_H
#define PURE2D_GRAPHICS_COLOR_H

#include <Pure2D/Math/Vec4.h>

namespace pure
{
    namespace color
    {
        constexpr Vec4f RED = { 1.f, 0.f, 0.f, 1.f };
        constexpr Vec4f GREEN = { 0.f, 1.f, 0.f, 1.f };
        constexpr Vec4f BLUE = { 0.f, 0.f, 1.f, 0.f };
        constexpr Vec4f WHITE = { 1.f, 1.f, 1.f, 1.f };
        constexpr Vec4f BLACK = { 0.f, 0.f, 0.f, 1.f };
    }
}

#endif // PURE2D_GRAPHICS_COLOR_H