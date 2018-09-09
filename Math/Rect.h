#ifndef PURE3D_MATH_RECT_H
#define PURE3D_MATH_RECT_H

#include <cinttypes>
#include <type_traits>

namespace pure
{
	template<typename T>
	struct Rect
	{
		static_assert(std::is_fundamental<T>::value, "Type of Rect must be primitive");

		T x, y, w, h;

		T bottom() const { return y + h; }
		T right() const { return x + w; }

	};

	using Rectf = Rect<float>;
	using Recti = Rect<int>;
	using Rectui = Rect<uint32_t>;
}

#endif // PURE3D_MATH_RECT_H
