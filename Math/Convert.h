#ifndef PURE2D_MATH_CONVERT_H
#define PURE2D_MATH_CONVERT_H

#include "Define.h"
#include "Constants.h"

namespace pure
{
	constexpr float radians(float degrees) 
	{ 
		return degrees * static_cast<float>(DEG_TO_RAD); 
	}

	constexpr float degrees(float radians) 
	{ 
		return radians * static_cast<float>(RAD_TO_DEG); 
	}
}

#endif // PURE2D_MATH_CONVERT_H