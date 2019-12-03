//
// Created by matt on 9/16/18.
//

#ifndef PURE2D_UTIL_H
#define PURE2D_UTIL_H

#include <algorithm>

#define PURE_ARRAY_COUNT(arr) (sizeof(arr) / sizeof(std::remove_pointer_t<std::decay<decltype(arr)>::type>))

namespace pure
{
    namespace util
    {

        constexpr float PI = 3.1415926535897932384626433832795f;
        constexpr float HALF_PI = 1.5707963267948966192313216916398f;
        constexpr float TWO_PI = 6.283185307179586476925286766559f;
        constexpr float DEG_TO_RAD = 0.017453292519943295769236907684886f;
        constexpr float RAD_TO_DEG = 57.295779513082320876798154814105f;
        constexpr float EULER = 2.718281828459045235360287471352f;

        constexpr float radians(float degrees) { return degrees * DEG_TO_RAD; }

        constexpr float degrees(float radians) { return radians * RAD_TO_DEG; }

        template<typename T>
        constexpr T& indexFlat2DArr(T* arr, int width, int x, int y)
        {
            return arr[x * width + y];
        }

        template <typename T>
        constexpr int signum(T val)
        {
            return (T(0) < val) - (val < T(0));
        }

        template <typename T>
        constexpr T clamp(T val, T minVal, T maxVal)
        {
            return std::min(std::max(val, minVal), maxVal);
        }

    }
}

#endif //PURE2D_UTIL_H
