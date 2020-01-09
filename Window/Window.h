//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_WINDOW_H
#define PURE2D_WINDOW_H


#include <cstdint>
#include <queue>
#include <Pure2D/Define.h>
#include <Pure2D/Window/WindowEvent.h>
#include <Pure2D/System/NonCopyable.h>
#include <Pure2D/Math/Vec4.h>
#include <Pure2D/Math/Vec2.h>


namespace pure
{

	template<typename T> struct Vec2;
	struct WindowEvent;
	enum class Key : uint32_t;

	struct PURE2D_API Window : private NonCopyable
	{
		friend struct WinEventHandler;

		int width() const;
		int height() const;
		Vec2i size() const;
		Vec2f mousePos() const;

		bool isOpen() const;
		bool isKeyPressed(Key key) const;

		void setSize(uint32_t width, uint32_t height);
		void setTitle(const char* title) const;

		bool make(const char* title);
		bool make(uint32_t width, uint32_t height, const char* title);
		void close() const;
		void destroy();

		void captureMouse() const;
		void releaseMouse() const;

		bool pollEvents(WindowEvent& e);

		void swapBuffers() const;

		void toggleFullscreenWindowed();

		void setVsync(bool isEnabled) const;

	private:
		int m_width;
		int m_height;
		bool m_isFullscreen;
		void* m_handle;
		std::queue<WindowEvent> m_events;

		Vec2f m_mousePos;

	};
}

#endif //PURE2D_WINDOW_H
