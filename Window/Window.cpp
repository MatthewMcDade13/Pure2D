//
// Created by matt on 9/3/18.
//
#include "Window.h"
#include "External/glad.h"
#include "Math/Vec2.h"
#include "WindowEvent.h"
#include "Private/Convert.h"
#include <queue>
#include <GLFW/glfw3.h>


#include <iostream>

#define glfwHandle(handle) (static_cast<GLFWwindow*>(handle))

using namespace pure;
static constexpr Vec2i DEFAULT_WIN_SIZE = { 1024, 768 };

namespace pure
{
	struct WinEventHandler
	{
		static void onResize(GLFWwindow* win, int width, int height);
		static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
		static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
		static void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}

static GLFWmonitor* getPrimaryMonitor();
static inline Vec2i getGLFWWindowSize(GLFWwindow* win);


void pure::Window::setSize(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;
	glfwSetWindowSize(glfwHandle(m_handle), width, height);
	glViewport(0, 0, m_width, m_height);
}

void pure::Window::setTitle(const char* title) const
{
	glfwSetWindowTitle(glfwHandle(m_handle), title);
}

bool pure::Window::make(const char* title)
{
	return make(DEFAULT_WIN_SIZE.x, DEFAULT_WIN_SIZE.y, title);
}

bool pure::Window::make(uint32_t width, uint32_t height, const char* title)
{
	GLFWwindow *const handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!handle)
	{
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(handle);
	glfwSetFramebufferSizeCallback(handle, WinEventHandler::onResize);
	glfwSetCursorPosCallback(handle, WinEventHandler::onMouseMove);
	glfwSetKeyCallback(handle, WinEventHandler::onKeyInput);
	glfwSetScrollCallback(handle, WinEventHandler::onMouseScroll);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return false;
	}


	m_handle = handle;
	m_width = width;
	m_height = height;
	m_isFullscreen = false;
	
	glfwSetWindowUserPointer(handle, this);

	glViewport(0, 0, m_width, m_height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void pure::Window::close() const
{
	glfwSetWindowShouldClose(glfwHandle(m_handle), true);
}

void pure::Window::destroy()
{
	glfwDestroyWindow(glfwHandle(m_handle));
	m_width = 0;
	m_height = 0;
	m_isFullscreen = false;
	m_handle = nullptr;
	m_mousePos = {};
	m_events = {};
}

bool pure::Window::isOpen() const
{
	return !glfwWindowShouldClose(glfwHandle(m_handle));
}

void pure::Window::setVsync(bool isEnabled) const { SDL_GL_SetSwapInterval(isEnabled); }

void pure::Window::captureMouse() const
{
	glfwSetInputMode(glfwHandle(m_handle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void pure::Window::releaseMouse() const 
{ 
	glfwSetInputMode(glfwHandle(m_handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool pure::Window::pollEvents(WindowEvent& e)
{
	glfwPollEvents();
	if (m_events.empty()) return false;

	e = m_events.front();
	m_events.pop();
	return true;
}

void pure::Window::swapBuffers() const
{
	glfwSwapBuffers(glfwHandle(m_handle));
}

bool pure::Window::isKeyPressed(Key key) const
{
	int keyState = glfwGetKey(glfwHandle(m_handle), static_cast<int>(key));
	return keyState == GLFW_PRESS;
};


void pure::Window::toggleFullscreenWindowed()
{
	GLFWmonitor* monitor = getPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	Vec2i winSize = {};

	// FIXME: When going from fullscreen to windowed, the coordinates get all messed up... still not 100% sure
	// if this is something that can be done here or its something the calling code will have to handle with
	// OpenGL
	if (m_isFullscreen)
	{
		// TODO: Maybe have Default Size be configurable?
		const Vec2i windowPos = {
				DEFAULT_WIN_SIZE.x / 2,
				DEFAULT_WIN_SIZE.y / 2
		};

		glfwSetWindowMonitor(glfwHandle(m_handle), nullptr,
			windowPos.x, windowPos.y, DEFAULT_WIN_SIZE.x,
			DEFAULT_WIN_SIZE.y, mode->refreshRate);
		m_isFullscreen = false;
	}
	else
	{
		glfwSetWindowMonitor(glfwHandle(m_handle), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		m_isFullscreen = true;
	}

}

int pure::Window::width() const { return m_width; }
int pure::Window::height() const { return m_height; }

Vec2i pure::Window::size() const
{
	return { m_width, m_height };
}

Vec2f pure::Window::mousePos() const { return m_mousePos; }


void WinEventHandler::onResize(GLFWwindow* win, int width, int height)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win));
	window->m_width = width;
	window->m_height = height;

	WindowEvent e;
	e.type = WindowEvent::Type::Resize;
	e.winSize = { window->m_width, window->m_height };
	window->m_events.push(e);

	glViewport(0, 0, window->m_width, window->m_height);
}


void WinEventHandler::onMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	win->m_mousePos = {
			static_cast<float>(xpos),
			static_cast<float>(ypos)
	};

	WindowEvent e;
	e.type = WindowEvent::Type::MouseMove;
	e.mousePos = { win->m_mousePos.x, win->m_mousePos.y };
	win->m_events.push(e);
}

void WinEventHandler::onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Ignoring repeat events for now...
	if (action == GLFW_REPEAT) return;

	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	WindowEvent e;

	if (action == GLFW_PRESS)
		e.type = WindowEvent::Type::KeyPress;
	else if (action == GLFW_RELEASE)
		e.type = WindowEvent::Type::KeyRelease;

	e.key = static_cast<Key>(key);

	win->m_events.push(e);
}

void WinEventHandler::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	WindowEvent e;
	e.type = WindowEvent::Type::MouseScroll;
	e.scrollOffset = {
			static_cast<float>(xoffset),
			static_cast<float>(yoffset)
	};

	win->m_events.push(e);
}

GLFWmonitor* getPrimaryMonitor()
{
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if (count == 0) return nullptr;
	return monitors[0];
}

inline Vec2i getGLFWWindowSize(GLFWwindow* win)
{
	int width, height;
	glfwGetWindowSize(win, &width, &height);
	return { width, height };
}
