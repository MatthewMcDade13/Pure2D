//
// Created by matt on 9/3/18.
//

#include "Window.h"
#include "glad.h"
#include "Math/Vec2.h"
#include "WindowEvent.h"

#include <queue>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace pure;

namespace pure
{
    struct WindowImpl
    {
        int width;
        int height;
        bool isFullscreen;
        GLFWwindow* handle;

        Vec2f mousePos;
        std::queue<WindowEvent> events;
    };
}

static constexpr Vec2i DEFAULT_WIN_SIZE = { 1024, 768 };

static void onResize(GLFWwindow* win, int width, int height);
static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
static void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
static GLFWmonitor* getPrimaryMonitor();
static inline Vec2i getGLFWWindowSize(GLFWwindow* win);

pure::Window::Window(): m_impl(new WindowImpl()) { }

pure::Window::~Window() { delete m_impl; }

int pure::Window::width() const { return m_impl->width; }
int pure::Window::height() const { return m_impl->height; }

Vec2f pure::Window::mousePos() const { return m_impl->mousePos; }

bool Window::create(const char *title)
{
    return create(DEFAULT_WIN_SIZE.x, DEFAULT_WIN_SIZE.y, title);
}

bool pure::Window::create(uint32_t width, uint32_t height, const char* title)
{

    GLFWwindow* handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!handle)
    {
        std::cerr << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(handle);
    glfwSetFramebufferSizeCallback(handle, onResize);
    glfwSetCursorPosCallback(handle, onMouseMove);
    glfwSetKeyCallback(handle, onKeyInput);
    glfwSetScrollCallback(handle, onMouseScroll);

    m_impl->handle = handle;
    m_impl->width = width;
    m_impl->height = height;
    m_impl->isFullscreen = false;

    glfwSetWindowUserPointer(handle, m_impl);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, m_impl->width, m_impl->height);
    glEnable(GL_DEPTH_TEST);

    return true;
}

void pure::Window::close() const
{
    glfwSetWindowShouldClose(m_impl->handle, true);
}

bool pure::Window::isOpen() const
{
    return !glfwWindowShouldClose(m_impl->handle);
}

void pure::Window::captureMouse() const
{
    glfwSetInputMode(m_impl->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void pure::Window::releaseMouse() const
{
    glfwSetInputMode(m_impl->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool pure::Window::pollEvents(WindowEvent& e)
{
    glfwPollEvents();
    if (m_impl->events.empty()) return false;

    e = m_impl->events.front();
    m_impl->events.pop();
    return true;
}

void pure::Window::swapBuffers() const
{
    glfwSwapBuffers(m_impl->handle);
}

bool pure::Window::isKeyPressed(Key key) const
{
    int keyState = glfwGetKey(m_impl->handle, static_cast<int>(key));
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
    if (m_impl->isFullscreen)
    {
        // TODO: Maybe have Default Size be configurable?
        const Vec2i windowPos = {
                DEFAULT_WIN_SIZE.x / 2,
                DEFAULT_WIN_SIZE.y / 2
        };

        glfwSetWindowMonitor(m_impl->handle, nullptr, windowPos.x, windowPos.y, DEFAULT_WIN_SIZE.x, DEFAULT_WIN_SIZE.y, mode->refreshRate);
        m_impl->isFullscreen = false;
    }
    else
    {
        glfwSetWindowMonitor(m_impl->handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        m_impl->isFullscreen = true;
    }
}

void pure::Window::clear(const Vec4f& clearColor) const
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pure::Window::setSize(uint32_t width, uint32_t height)
{
    m_impl->width = width;
    m_impl->height = height;
    glfwSetWindowSize(m_impl->handle, width, height);
}

void onResize(GLFWwindow * win, int width, int height)
{
    WindowImpl* window = static_cast<WindowImpl*>(glfwGetWindowUserPointer(win));
    window->width = width;
    window->height = height;

    WindowEvent e;
    e.type = WindowEvent::Type::Resize;
    e.winSize = { window->width, window->height };
    window->events.push(e);

    glViewport(0, 0, window->width, window->height);
}


void onMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    WindowImpl* win = static_cast<WindowImpl*>(glfwGetWindowUserPointer(window));

    win->mousePos = {
            static_cast<float>(xpos),
            static_cast<float>(ypos)
    };

    WindowEvent e;
    e.type = WindowEvent::Type::MouseMove;
    e.mousePos = { win->mousePos.x, win->mousePos.y };
    win->events.push(e);
}

void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Ignoring repeat events for now...
    if (action == GLFW_REPEAT) return;

    WindowImpl* win = static_cast<WindowImpl*>(glfwGetWindowUserPointer(window));

    WindowEvent e;

    if (action == GLFW_PRESS)
        e.type = WindowEvent::Type::KeyPress;
    else if (action == GLFW_RELEASE)
        e.type = WindowEvent::Type::KeyRelease;

    e.key = static_cast<Key>(key);

    win->events.push(e);
}

GLFWmonitor * getPrimaryMonitor()
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

void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    WindowImpl* win = static_cast<WindowImpl*>(glfwGetWindowUserPointer(window));

    WindowEvent e;
    e.type = WindowEvent::Type::MouseScroll;
    e.scrollOffset = {
            static_cast<float>(xoffset),
            static_cast<float>(yoffset)
    };

    win->events.push(e);
}
