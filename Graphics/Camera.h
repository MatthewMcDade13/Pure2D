<<<<<<< HEAD
#ifndef PURE2D_CAMERA_H
#define PURE2D_CAMERA_H

#include <Pure2D/Define.h>
#include <Pure2D/Math/Vec3.h>

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


#endif // PURE2D_CAMERA_H
=======
#ifndef PURE2D_CAMERA_H
#define PURE2D_CAMERA_H

#include <Pure2D/Define.h>
#include <Pure2D/Math/Vec3.h>

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


#endif // PURE2D_CAMERA_H
>>>>>>> 122077b860bcf9e03fd9e59de88d008efff1773b
