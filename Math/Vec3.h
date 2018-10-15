//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_VEC3_H
#define PURE2D_VEC3_H


#include <cinttypes>
#include <type_traits>
#include <Pure2D/Math/Vec2.h>
#include "MatrixTransform.h"


namespace pure
{
    template<typename T>
    struct Vec3
    {
        static_assert(std::is_fundamental<T>::value, "Type of Vector must be primitive");

        union
        {
            struct { T x, y, z; };
            struct { T r, g, b; };
        };

        template<typename U>
        explicit constexpr operator Vec3<U>() const { return { U(x), U(y), U(z) }; }

        constexpr Vec3(Vec2<T> a, T z = T(0.0))
                : x(a.x), y(a.y), z(z) { }

        constexpr Vec3(T x = T(0.0), T y = T(0.0), T z = T(0.0))
                : x(x), y(y), z(z) { }

        Vec3<T> static constexpr single(T val) { return { val, val, val }; }
        Vec3<T> static constexpr UP() { return { T(0.0), T(1.0), T(0.0) }; }
        Vec3<T> static constexpr FORWARD() { return { T(0.0), T(0.0), T(1.0) }; }

        Vec3<T>& operator+=(const Vec3<T>& vec)
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            return *this;
        }

        Vec3<T>& operator+=(T scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }


        Vec3<T>& operator-=(const Vec3<T>& vec)
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            return *this;
        }

        Vec3<T>& operator-=(T scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        Vec3<T>& operator*=(const Vec3<T>& vec)
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            return *this;
        }

        Vec3<T>& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        Vec3<T>& operator/=(const Vec3<T>& vec)
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
            return *this;
        }

        Vec3<T>& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        static constexpr Vec3<T> EMPTY()
        {
            return { T(), T(), T() };
        }
    };

    using Vec3f = Vec3<float>;
    using Vec3i = Vec3<int>;
    using Vec3ui = Vec3<uint32_t>;

    template<typename T>
    constexpr Vec3<T> operator-(const Vec3<T>& vec)
    {
        return {
                -vec.x,
                -vec.y,
                -vec.z
        };
    }

    template<typename T>
    constexpr Vec3<T> operator+(const Vec3<T>& vec, T scalar)
    {
        return {
                vec.x + scalar,
                vec.y + scalar,
                vec.z + scalar
        };
    }

    template<typename T>
    constexpr Vec3<T> operator+(T scalar, const Vec3<T>& vec)
    {
        return {
                scalar + vec.x,
                scalar + vec.y,
                scalar + vec.z
        };
    }

    template<typename T>
    constexpr Vec3<T> operator+(const Vec3<T>& left, const Vec3<T>& right)
    {
        return {
                left.x + right.x,
                left.y + right.y,
                left.z + right.z
        };
    }


    template <typename T>
    constexpr Vec3<T> operator-(const Vec3<T>& vec, T scalar)
    {
        return {
                vec.x - scalar,
                vec.y - scalar,
                vec.z - scalar
        };
    }

    template <typename T>
    constexpr Vec3<T> operator-(T scalar, const Vec3<T>& vec)
    {
        return {
                scalar - vec.x,
                scalar - vec.y,
                scalar - vec.z
        };
    }


    template <typename T>
    constexpr Vec3<T> operator-(const Vec3<T>& left, const Vec3<T>& right)
    {
        return {
                left.x - right.x,
                left.y - right.y,
                left.z - right.z
        };
    }

    template <typename T>
    constexpr Vec3<T> operator*(const Vec3<T>& vec, T scalar)
    {
        return {
                vec.x * scalar,
                vec.y * scalar,
                vec.z * scalar
        };
    }

    template <typename T>
    constexpr Vec3<T> operator*(T scalar, const Vec3<T>& vec)
    {
        return {
                scalar * vec.x,
                scalar * vec.y,
                scalar * vec.z
        };
    }

    template <typename T>
    constexpr Vec3<T> operator*(const Vec3<T>& left, const Vec3<T>& right)
    {
        return {
                left.x * right.x,
                left.y * right.y,
                left.z * right.z
        };
    }

    template <typename T>
    constexpr Vec3<T> operator/(const Vec3<T>& vec, T scalar)
    {
        return {
                vec.x / scalar,
                vec.y / scalar,
                vec.z / scalar
        };
    }

    template <typename T>
    constexpr Vec3<T> operator/(T scalar, const Vec3<T>& vec)
    {
        return {
                scalar / vec.x,
                scalar / vec.y,
                scalar / vec.z
        };
    }

    template <typename T>
    constexpr Vec3<T> operator/(const Vec3<T>& left, const Vec3<T>& right)
    {
        return {
                left.x / right.x,
                left.y / right.y,
                left.z / right.z
        };
    }

    template <typename T>
    constexpr bool operator==(const Vec3<T>& left, const Vec3<T>& right)
    {
        return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
    }

    template <typename T>
    constexpr bool operator!=(const Vec3<T>& left, const Vec3<T>& right)
    {
        return !(left == right);
    }
}


#endif //PURE2D_VEC3_H
