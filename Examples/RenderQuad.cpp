#include <Pure2D/Engine/Game.h>
#include <Pure2D/Graphics.h>
#include <Pure2D/System/Logging.h>
#include <iostream>

static constexpr const char* SPOT_EFFECT = "\n"
"uniform vec2 mousePos;\n"
"const float radius = 100.0;\n"
"const float radSquared = radius * radius;\n"

	"vec4 effect(vec4 color, sampler2D tex, vec2 texCoords, vec3 fragPos)\n"
	"{\n"
	"float dx = fragPos.x - mousePos.x;\n"
	"float dy = fragPos.y - mousePos.y;\n"
	"float dist = (dx * dx) + (dy * dy);\n"

	"if (dist <= radSquared)\n"
		"return texture(tex, texCoords) * vec4(1.0, 0.0, 0.0, 0.5);\n"

	"return texture(tex, texCoords);\n"
"}\n";


/**

	Pres B key while running to enlarge quad
	Press S key while quad is enlarged to put it back to original shape

	Move your mouse around the quad, you will see a circle made with a post-processing shader
*/
struct HoverSpot
{

	static constexpr int mouseUniformLoc = pure::Shader::DEFAULT_LOC_COUNT;
	pure::Transform transform = pure::Transform::make();
	pure::Quad body = pure::Quad::make();
	pure::Shader shader;
};

struct QuadExample final : public pure::Game
{
	pure::Quad q = pure::Quad::make();
	pure::Transform qxform = pure::Transform::make();
	HoverSpot hoverSpot;
	pure::RenderTexture rt;

	~QuadExample()
	{
		rt.free();
	}

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

		hoverSpot.shader = pure::Shader::fromTemplate(nullptr, SPOT_EFFECT);
		hoverSpot.shader.locations.push_back(hoverSpot.shader.getLocation("mousePos"));
		rt = pure::RenderTexture::make(m_window.width(), m_window.height());
	}

	void draw() final
	{
		std::cout << "BRUH" << std::endl;
		// Start drawing to framebuffer
		m_renderer.beginDrawTexture(rt);
		{
			m_renderer.clear();
			m_renderer.drawQuad(q, &qxform.modelMatrix());
		}
		m_renderer.endDrawTexture();


		const pure::Vec3f camPos = m_renderer.cam.position;
		m_renderer.cam.position = {};

		// Post Processing
		hoverSpot.transform.setPosition({ 0.f, 0.f,  5.f });
		hoverSpot.transform.setSize({ float(m_window.width()), float(m_window.height()) });
		m_renderer.drawQuad(hoverSpot.body, &hoverSpot.transform.modelMatrix(), hoverSpot.shader, &rt.texture);


		m_renderer.cam.position = camPos;
	}

	void update(float dt) final
	{
		hoverSpot.shader.setUniformIndx(HoverSpot::mouseUniformLoc, m_window.mousePos());
	}

	void windowEvent(pure::WindowEvent& event) final
	{
		if (event.type == pure::WindowEvent::Type::MouseMove)
		{
			const pure::Vec2f mousePos = event.mousePos.toVec2();

			const pure::Vec3f worldPos = screenToWorldPos(mousePos);
			const pure::Vec2i pos = static_cast<pure::Vec2i>(pure::Vec2f(worldPos.x, worldPos.y));

			hoverSpot.transform.setPosition({ static_cast<pure::Vec2f>(pos)* pure::Vec2f::single(50.f) });
		}
		if (event.type == pure::WindowEvent::Type::KeyRelease)
		{
			switch (event.key)
			{
			case pure::Key::Escape:
				m_window.close();
				break;
			case pure::Key::B:
				qxform.setSize(float(m_window.width()), float(m_window.height()));
				qxform.setPosition({ 0.f, 0.f, 0.5f });
				break;
			case pure::Key::S:
				qxform.setSize({ 500.f, 500.f });
				qxform.setPosition({
					(static_cast<float>(m_window.width()) * .5f) - (qxform.size().x * .5f),
					(static_cast<float>(m_window.height()) * .5f) - (qxform.size().y * .5f),
					0.5f
					});
				break;
			}
		}
		if (event.type == pure::WindowEvent::Type::MouseScroll)
		{
			m_renderer.zoom(-event.scrollOffset.y * 50.f);
		}
	}
};

int main()
{
	QuadExample g = {};
	g.run();
	return 0;
}

