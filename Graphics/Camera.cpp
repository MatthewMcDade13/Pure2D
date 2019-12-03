#include "Camera.h"
#include "Math/Mat4.h"
#include "Math/Manip.h"
#include "Math/Convert.h"
#include "Math/MatrixTransform.h"
#include <cmath>

using namespace pure;

Mat4 pure::Camera::view() const
{
	return lookAt(position, target(), Vec3f::UP());
}

Vec3f pure::Camera::target() const
{
	return position + lookDir;
}

void pure::Camera::moveForward(float dt)
{
	position += speed * lookDir * dt;
}

void pure::Camera::moveBackward(float dt)
{
	position -= speed * lookDir * dt;
}

void pure::Camera::moveLeft(float dt)
{
	position -= normalize(cross(lookDir, Vec3f::UP())) * speed * dt;
}

void pure::Camera::moveRight(float dt)
{
	position += normalize(cross(lookDir, Vec3f::UP())) * speed * dt;
}

void pure::Camera::lookTowards(const Vec3f & offset)
{
	const Vec3f delta = offset * sensitivity;

	rotation += Vec3f(delta.x, delta.y);
	rotation.y = clamp(rotation.y, -89.f, 89.f);

	Vec3f front = {};
	front.x = cosf(radians(rotation.x)) * cosf(radians(rotation.y));
	front.y = sinf(radians(rotation.y));
	front.z = sinf(radians(rotation.x)) * cosf(radians(rotation.y));
	lookDir = normalize(front);
}
