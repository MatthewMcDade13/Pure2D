#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Pure2D/Init.h>
#include <Pure2D/Pure2D.h>
#include <Pure2D/System/Util.h>
#include <Pure2D/Graphics/Mesh.h>
#include <Pure2D/Graphics/Quad.h>
#include "Private/Convert.h"

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

	c.init();
	while (m_window.isOpen())
	{
		while (m_window.pollEvents(event))
		{
			windowEvent(event);
		}

		const float dt = c.reset();

		update(dt);

		m_window.clear();

		draw();

		m_window.swapBuffers();
	}
}

pure::Vec3f pure::Game::screenToWorldPos(Vec2f screenPos) const
{
	const Mat4 pvMat = inverse(m_renderer.projection() * m_renderer.cam.view());
	Vec2f temp = {
		(2.f * (screenPos.x / m_window.width())) - 1.f,
		// mousePos.x / win.width(),
		1.f - (2.f * (screenPos.y / m_window.height()))
	};
	const Vec4f pos = pvMat * Vec4f(temp, 0.f, 1.f);

	return Vec3f(pos.x, pos.y, pos.z);
}
