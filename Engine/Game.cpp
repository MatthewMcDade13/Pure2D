#include "Game.h"
#include "External/imgui/imgui.h"
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
	m_window.make(DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y, "Pure2D");
	m_renderer.create(m_window);
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
		m_renderer.newImGuiFrame();

		if (showFPS)
		{
			ImGui::Begin("Average FPS", &showFPS);
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		const float dt = c.reset();

		update(dt);

		m_renderer.clear();

		draw();

		m_renderer.present();
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
