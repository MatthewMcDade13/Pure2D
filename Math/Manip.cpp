#include "Manip.h"
#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include "Private/Convert.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"

using namespace pure;

Vec2<float> pure::normalize(const Vec2<float>& vec)
{
	return vec / static_cast<float>(sqrt(dot(vec, vec)));
}

Vec3<float> pure::normalize(const Vec3<float>& vec)
{
	return vec / static_cast<float>(sqrt(dot(vec, vec)));
}

Vec4<float> pure::normalize(const Vec4<float>& vec)
{
	return vec / static_cast<float>(sqrt(dot(vec, vec)));
}

Vec3<float> pure::cross(const Vec3<float>& lvec, const Vec3<float>& rvec)
{
	return {
		(lvec.y * rvec.z) - (lvec.z * rvec.y),
		(lvec.z * rvec.x) - (lvec.x * rvec.z),
		(lvec.x * rvec.y) - (lvec.y * rvec.x)
	};
}

float pure::dot(const Vec2<float>& lvec, const Vec2<float>& rvec)
{
	Vec2f result = lvec * rvec;
	return result.x + result.y;
}

float pure::dot(const Vec3<float>& lvec, const Vec3<float>& rvec)
{
	Vec3f result = lvec * rvec;
	return result.x + result.y + result.z;
}

float pure::dot(const Vec4<float>& lvec, const Vec4<float>& rvec)
{
	Vec4f result = lvec * rvec;
	return result.x + result.y + result.z + result.w;
}

float pure::clamp(float a, float min, float max)
{
	return std::min(std::max(a, min), max);
}
