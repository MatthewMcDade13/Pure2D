#ifndef PURE2D_MATH_RECT_H
#define PURE2D_MATH_RECT_H

#include <cinttypes>
#include <type_traits>
#include <Pure2D/Math/Vec4.h>

namespace pure
{
	template<typename T>
	struct Rect
	{
		static_assert(std::is_fundamental<T>::value, "Type of Rect must be primitive");

		explicit constexpr operator Vec4<T>() const { return { T(x), T(y), T(w), T(h) }; }

		template<typename U>
		explicit constexpr operator Rect<U>() const { return { U(x), U(y), U(w), U(h) }; }

		T x, y, w, h;

		T bottom() const { return y + h; }
		T right() const { return x + w; }

	};

	using Rectf = Rect<float>;
	using Recti = Rect<int>;
	using Rectui = Rect<uint32_t>;
}

#endif // PURE2D_MATH_RECT_H
