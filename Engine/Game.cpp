#include "Game.h"
#include <Pure2D/Init.h>
#include <Pure2D/Pure2D.h>
#include <Pure2D/System/Util.h>
#include <Pure2D/Graphics/Mesh.h>
#include <Pure2D/Graphics/Quad.h>

constexpr uint32_t DEFAULT_SCREEN_X = 1024;
constexpr uint32_t DEFAULT_SCREEN_Y = 768;

pure::Game::Game()
{
	pure::init();
	m_window.create(DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y, "");
	m_renderer.create();
}

pure::Game::~Game()
{
	m_renderer.destroy();
	m_window.destroy();
	pure::terminate();
}

void pure::Game::run()
{
	WindowEvent event = {};

	time::Clock c = {};

	load();

	time::initClock(c);
	while (m_window.isOpen())
	{
		while (m_window.pollEvents(event))
		{
			windowEvent(event);
		}

		const float dt = time::resetClock(c);

		update(dt);

		m_window.clear();

		draw();

		m_window.swapBuffers();
	}
}
