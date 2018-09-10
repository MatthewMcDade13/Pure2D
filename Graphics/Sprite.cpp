#include "Sprite.h"
#include "Math/Vec3.h"
#include "Graphics/Texture.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"

using namespace pure;

Vec3f pure::Sprite::position() const { return m_position; }
Vec2f pure::Sprite::size() const { return m_size; }

float pure::Sprite::rotation() const { return m_rotation; }

const Texture* pure::Sprite::texture() const { return m_texture; }

// TODO: Create Transform struct that will encapsulate this exact functionality
const Mat4 & pure::Sprite::modelMatrix()
{
	if (m_needsUpdate)
	{
		m_model = translate(makeMat4(), m_position);
		m_model = translate(m_model, Vec3f(m_size.x * .5f, m_size.y * .5f, 0.f));
		m_model = pure::rotate(m_model, radians(m_rotation), Vec3f(0.f, 0.f, 1.f));
		m_model = translate(m_model, Vec3f(m_size.x * -.5f, m_size.y * -.5f, 0.f));
		m_model = scale(m_model, Vec3f(m_size.x, m_size.y, 1.f));
		m_needsUpdate = false;
	}

	return m_model;
}

void pure::Sprite::setPosition(Vec3f pos)
{
	m_position = pos;
	m_needsUpdate = true;
}

void pure::Sprite::setSize(Vec2f size)
{
	m_size = size;
	m_needsUpdate = true;
}

void pure::Sprite::setRotation(float angle)
{
	m_rotation = angle;
	m_needsUpdate = true;
}

void pure::Sprite::setTexture(Texture* tex)
{
	m_texture = tex;
	textureRect = { 0, 0,
		static_cast<uint32_t>(m_texture->size.x),
		static_cast<uint32_t>(m_texture->size.y),
	};
}

void pure::Sprite::move(Vec3f offset)
{
	m_position += offset;
	m_needsUpdate = true;
}

void pure::Sprite::rotate(float angle)
{
	m_rotation += angle;
	m_needsUpdate = true;
}
