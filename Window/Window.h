//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_WINDOW_H
#define PURE2D_WINDOW_H


#include <cstdint>
#include <Pure2D/Define.h>
#include <Pure2D/System/NonCopyable.h>
#include <Pure2D/Math/Vec4.h>

namespace pure
{
    template<typename T> struct Vec2;
    struct WindowEvent;
    enum class Key : uint32_t;

    struct PURE2D_API Window : private NonCopyable
    {
        Window();

        ~Window();

        int width() const;
        int height() const;
		Vec2i size() const;
        Vec2<float> mousePos() const;
        bool isOpen() const;

        void setSize(uint32_t width, uint32_t height);
        void setTitle(const char* title) const;

        bool create(const char* title);
        bool create(uint32_t width, uint32_t height, const char* title);
        void close() const;
		void destroy();

        void captureMouse() const;
        void releaseMouse() const;

        bool pollEvents(WindowEvent& e);

        void clear(const Vec4<float>& clearColor = Vec4<float>::single(0.f)) const;
        void swapBuffers() const;

        bool isKeyPressed(Key key) const;

        void toggleFullscreenWindowed();
    private:
        struct WindowImpl* m_impl;
    };

}

#endif //PURE2D_WINDOW_H
