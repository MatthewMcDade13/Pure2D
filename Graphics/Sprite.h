#ifndef PURE3D_SPRITE_H
#define PURE3D_SPRITE_H

#include "Transform.h"
#include "Math/Rect.h"
#include "Define.h"

namespace pure
{
	struct Texture;

	struct Sprite
	{
		Rectui textureRect;

		PURE2D_API Vec3f position() const;
		PURE2D_API Vec2f size() const;
		PURE2D_API float rotation() const;
		PURE2D_API const Texture* texture() const;

		PURE2D_API const Mat4& modelMatrix();

		PURE2D_API void setPosition(const Vec3f& pos);
		PURE2D_API void setSize(Vec2f size);
		PURE2D_API void setRotation(float angle);
		PURE2D_API void setTexture(Texture* tex);

        PURE2D_API void move(const Vec3f& offset);
        PURE2D_API void scale(Vec2f offset);
        PURE2D_API void rotate(float angle);

	private:
		Texture* m_texture;
		Transform m_transform;

	};
}

#endif // PURE3D_SPRITE_H
