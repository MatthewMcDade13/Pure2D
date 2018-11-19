//
// Created by matt on 9/3/18.
//
#include "Window.h"
#include "glad.h"
#include "Math/Vec2.h"
#include "WindowEvent.h"
#include "Private/Convert.h"
#include <SDL2/SDL.h>

#include <iostream>

#define sdlwin(handle) (static_cast<SDL_Window*>(handle))

using namespace pure;
static constexpr Vec2i DEFAULT_WIN_SIZE = { 1024, 768 };

void pure::Window::setSize(uint32_t width, uint32_t height)
{
	SDL_SetWindowSize(sdlwin(m_handle), width, height);
	m_width  = width;
	m_height = height;
}

void pure::Window::setTitle(const char* title) const
{
	SDL_SetWindowTitle(sdlwin(m_handle), title);
}

bool pure::Window::create(const char* title)
{
	return create(DEFAULT_WIN_SIZE.x, DEFAULT_WIN_SIZE.y, title);
}

bool pure::Window::create(uint32_t width, uint32_t height, const char* title)
{
	m_handle = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);

	if (!m_handle)
	{
		std::cerr << "SDL :: Failed to create window" << std::endl;
		return false;
	}

	m_ctx = SDL_GL_CreateContext(sdlwin(m_handle));

	if (!m_ctx)
	{
		std::cerr << "SDL :: Failed to create OpenGL context" << std::endl;
		SDL_DestroyWindow(sdlwin(m_handle));
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cerr << "GLAD :: Failed to initialize" << std::endl;
		SDL_DestroyWindow(sdlwin(m_handle));
		SDL_GL_DeleteContext(m_ctx);
		return false;
	}

	m_width = width;
	m_height = height;
	m_isFullscreen = false;
	isActive = true;
	
	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	return true;
}

void pure::Window::destroy()
{
	SDL_DestroyWindow(sdlwin(m_handle));
	m_handle = nullptr;

	SDL_GL_DeleteContext(m_ctx);
	m_ctx = nullptr;
}

void pure::Window::captureMouse() const { SDL_CaptureMouse(SDL_TRUE); }
void pure::Window::releaseMouse() const { SDL_CaptureMouse(SDL_FALSE); }

bool pure::Window::pollEvents(WindowEvent& e)
{
	SDL_Event sdlEvent;
	const bool isPendingEvents = SDL_PollEvent(&sdlEvent);

	switch (sdlEvent.type)
	{
	case SDL_KEYDOWN: 
		e.type = WindowEvent::Type::KeyPress;
	case SDL_KEYUP: {
		e.type = WindowEvent::Type::KeyRelease;
		e.key = toPureKey(sdlEvent.key.keysym.scancode);
	} break;
	case SDL_MOUSEMOTION: {
		e.type = WindowEvent::Type::MouseMove;
		m_mousePos = {
			static_cast<float>(sdlEvent.motion.x),
			static_cast<float>(sdlEvent.motion.y)
		};
		e.mousePos = { m_mousePos.x, m_mousePos.y };
	} break;
	case SDL_MOUSEWHEEL: {
		e.type = WindowEvent::Type::MouseScroll;
		e.scrollOffset = {
			static_cast<float>(sdlEvent.wheel.x),
			static_cast<float>(sdlEvent.wheel.y)
		};
	} break;
	case SDL_WINDOWEVENT: {
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				m_width = sdlEvent.window.data1;
				m_height = sdlEvent.window.data2;

				e.type = WindowEvent::Type::Resize;
				e.winSize = { m_width, m_height };
				glViewport(0, 0, m_width, m_height);

			}
		} break;
	}

	return isPendingEvents;
}

void pure::Window::clear(const Vec4<float>& clearColor) const
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pure::Window::swapBuffers() const
{
	SDL_GL_SwapWindow(sdlwin(m_handle));
}

void pure::Window::toggleFullscreenWindowed()
{
	m_isFullscreen = !m_isFullscreen;
	SDL_SetWindowFullscreen(sdlwin(m_handle), m_isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

int pure::Window::width() const { return m_width; }
int pure::Window::height() const { return m_height; }

Vec2i pure::Window::size() const
{
	return { m_width, m_height };
}

Vec2f pure::Window::mousePos() const { return m_mousePos; }
