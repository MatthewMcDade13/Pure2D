//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_WINDOWEVENT_H
#define PURE2D_WINDOWEVENT_H


#include <Pure2D/Window/Keyboard.h>
#include <Pure2D/Math/Vec2.h>
#include <new>

namespace pure
{
    template<typename T>
    struct Vec2;

    struct WindowEvent
    {
        struct Mouse
        {
            float x, y;
            inline Vec2f toVec2() { return { x, y }; }
        };

        struct Size
        {
            int x, y;
            inline Vec2i toVec2() { return { x, y }; }
        };

        struct Scroll
        {
            float x, y;
            inline Vec2f toVec2() { return { x, y }; }
        };

        enum class Type
        {
            KeyPress,
            KeyRelease,
            MouseMove,
            MouseScroll,
            Resize
        };

        Type type;

        union
        {
            Key key;
            Mouse mousePos;
            Size winSize;
            Scroll scrollOffset;
        };
    };

}


#endif //PURE2D_WINDOWEVENT_H
