#include "Sprite.h"
#include "Math/Vec3.h"
#include "Graphics/Texture.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"

using namespace pure;

Vec3f pure::Sprite::position() const { return m_transform.position(); }
Vec2f pure::Sprite::size() const { return m_transform.size(); }

float pure::Sprite::rotation() const { return m_transform.rotation(); }

const Texture* pure::Sprite::texture() const { return m_texture; }

// TODO: Create Transform struct that will encapsulate this exact functionality
const Mat4 & pure::Sprite::modelMatrix()
{
	return m_transform.modelMatrix();
}

void pure::Sprite::setPosition(const Vec3f& pos)
{
	m_transform.setPosition(pos);
}

void pure::Sprite::setSize(Vec2f size)
{
	m_transform.setSize(size);
}

void pure::Sprite::setRotation(float angle)
{
	m_transform.setRotation(angle);
}

void pure::Sprite::setTexture(Texture* tex)
{
	m_texture = tex;
	textureRect = { 0, 0,
		static_cast<uint32_t>(m_texture->size.x),
		static_cast<uint32_t>(m_texture->size.y),
	};
}

void pure::Sprite::move(const Vec3f &offset)
{
	m_transform.move(offset);
}

void pure::Sprite::scale(Vec2f offset)
{
	m_transform.scale(offset);
}

void pure::Sprite::rotate(float angle)
{
	m_transform.rotate(angle);
}
