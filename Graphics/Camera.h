#ifndef PURE3D_CAMERA_H
#define PURE3D_CAMERA_H

#include "Define.h"
#include "Math/Vec3.h"

namespace pure
{
	struct Mat4;

	struct Camera
	{
		Vec3f position;
		Vec3f lookDir;
		Vec3f rotation;

		float speed;
		float sensitivity;

		PURE2D_API Mat4 view() const;
		PURE2D_API Vec3f target() const;

		PURE2D_API void moveForward(float dt = 1.f);
		PURE2D_API void moveBackward(float dt = 1.f);
		PURE2D_API void moveLeft(float dt = 1.f);
		PURE2D_API void moveRight(float dt = 1.f);
		PURE2D_API void lookTowards(const Vec3f& offset);

	};

}


#endif // PURE3D_CAMERA_H
