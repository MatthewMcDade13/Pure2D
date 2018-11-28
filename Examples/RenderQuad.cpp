#include <Pure2D/Engine/Game.h>
#include <Pure2D/Graphics.h>

struct QuadExample final : public pure::Game
{
	pure::Quad q = pure::Quad::make();
	pure::Transform qxform = pure::Transform::make();

	void load() final
	{
		m_window.setTitle("Quad Example");

		q.verts[0].color = { 1.f, 0.f, 0.f, 1.f };
		q.verts[1].color = { 0.f, 1.f, 0.f, 1.f };
		q.verts[2].color = { 0.f, 0.f, 1.f, 1.f };
		q.verts[3].color = { 1.f, 1.f, 1.f, 1.f };

		qxform.setSize({ 500.f, 500.f });

		qxform.setPosition({ 
			(static_cast<float>(m_window.width()) * .5f) - (qxform.size().x * .5f),
			(static_cast<float>(m_window.height()) * .5f) - (qxform.size().y * .5f),
			0.5f
		});
	}

	void draw() final
	{
		m_renderer.drawQuad(q, &qxform.modelMatrix());
	}
};

int main()
{
	QuadExample g = {};
	g.run();
    return 0;
}

