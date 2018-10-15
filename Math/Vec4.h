//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_VEC4_H
#define PURE2D_VEC4_H


#include <cinttypes>
#include <type_traits>
#include <Pure2D/Math/Vec2.h>

namespace pure
{
    template<typename T> struct Vec3;

    template<typename T>
    struct Vec4
    {
        static_assert(std::is_fundamental<T>::value, "Type of Vector must be primitive");

        union
        {
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
        };

        template<typename U>
        explicit constexpr operator Vec4<U>() const { return { U(x), U(y), U(z), U(w) }; }

        constexpr Vec4(const Vec3<T>& a, T w = T(0.0))
                : x(a.x), y(a.y), z(a.z), w(w) { }

        constexpr Vec4(Vec2<T> a, T z, T w)
                : x(a.x), y(a.y), z(z), w(w) { }

        constexpr Vec4(Vec2<T> a, Vec2<T> b = {})
                : x(a.x), y(a.y), z(a.x), w(a.y) { }

        constexpr Vec4(T x = T(0.0), T y = T(0.0), T z = T(0.0), T w = T(0.0))
                : x(x), y(y), z(z), w(w) { }

        Vec4<T> static constexpr single(T val) { return { val, val, val, val }; }

        Vec4<T>& operator+=(const Vec4<T>& vec)
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            w += vec.w;
            return *this;
        }

        Vec4<T>& operator+=(T scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            w += scalar;
            return *this;
        }


        Vec4<T>& operator-=(const Vec4<T>& vec)
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            w -= vec.w;
            return *this;
        }

        Vec4<T>& operator-=(T scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            w -= scalar;
            return *this;
        }

        Vec4<T>& operator*=(const Vec4<T>& vec)
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            w *= vec.w;
            return *this;
        }

        Vec4<T>& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        Vec4<T>& operator/=(const Vec4<T>& vec)
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
            w /= vec.w;
            return *this;
        }

        Vec4<T>& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        static constexpr Vec4<T> EMPTY()
        {
            return { T(), T(), T(), T() };
        }
    };

    using Vec4f = Vec4<float>;
    using Vec4i = Vec4<int>;
    using Vec4ui = Vec4<uint32_t>;

    template<typename T>
    constexpr Vec4<T> operator-(const Vec4<T>& vec)
    {
        return {
                -vec.x,
                -vec.y,
                -vec.z,
                -vec.w
        };
    }

    template<typename T>
    constexpr Vec4<T> operator+(const Vec4<T>& vec, T scalar)
    {
        return {
                vec.x + scalar,
                vec.y + scalar,
                vec.z + scalar,
                vec.w + scalar
        };
    }

    template<typename T>
    constexpr Vec4<T> operator+(T scalar, const Vec4<T>& vec)
    {
        return {
                scalar + vec.x,
                scalar + vec.y,
                scalar + vec.z,
                scalar + vec.w
        };
    }

    template<typename T>
    constexpr Vec4<T> operator+(const Vec4<T>& left, const Vec4<T>& right)
    {
        return {
                left.x + right.x,
                left.y + right.y,
                left.z + right.z,
                left.w + right.w
        };
    }


    template <typename T>
    constexpr Vec4<T> operator-(const Vec4<T>& vec, T scalar)
    {
        return {
                vec.x - scalar,
                vec.y - scalar,
                vec.z - scalar,
                vec.w - scalar
        };
    }

    template <typename T>
    constexpr Vec4<T> operator-(T scalar, const Vec4<T>& vec)
    {
        return {
                scalar - vec.x,
                scalar - vec.y,
                scalar - vec.z,
                scalar - vec.w
        };
    }


    template <typename T>
    constexpr Vec4<T> operator-(const Vec4<T>& left, const Vec4<T>& right)
    {
        return {
                left.x - right.x,
                left.y - right.y,
                left.z - right.z,
                left.w - right.w
        };
    }

    template <typename T>
    constexpr Vec4<T> operator*(const Vec4<T>& vec, T scalar)
    {
        return {
                vec.x * scalar,
                vec.y * scalar,
                vec.z * scalar,
                vec.w * scalar
        };
    }

    template <typename T>
    constexpr Vec4<T> operator*(T scalar, const Vec4<T>& vec)
    {
        return {
                scalar * vec.x,
                scalar * vec.y,
                scalar * vec.z,
                scalar * vec.w
        };
    }

    template <typename T>
    constexpr Vec4<T> operator*(const Vec4<T>& left, const Vec4<T>& right)
    {
        return {
                left.x * right.x,
                left.y * right.y,
                left.z * right.z,
                left.w * right.w
        };
    }

    template <typename T>
    constexpr Vec4<T> operator/(const Vec4<T>& vec, T scalar)
    {
        return {
                vec.x / scalar,
                vec.y / scalar,
                vec.z / scalar,
                vec.w / scalar
        };
    }

    template <typename T>
    constexpr Vec4<T> operator/(T scalar, const Vec4<T>& vec)
    {
        return {
                scalar / vec.x,
                scalar / vec.y,
                scalar / vec.z,
                scalar / vec.w
        };
    }

    template <typename T>
    constexpr Vec4<T> operator/(const Vec4<T>& left, const Vec4<T>& right)
    {
        return {
                left.x / right.x,
                left.y / right.y,
                left.z / right.z,
                left.w / right.w
        };
    }

    template <typename T>
    constexpr bool operator==(const Vec4<T>& left, const Vec4<T>& right)
    {
        return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
    }

    template <typename T>
    constexpr bool operator!=(const Vec4<T>& left, const Vec4<T>& right)
    {
        return !(left == right);
    }
}


#endif //PURE2D_VEC4_H
