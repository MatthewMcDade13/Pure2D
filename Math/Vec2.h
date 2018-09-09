//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_VEC2_H
#define PURE2D_VEC2_H


#include <cinttypes>
#include <type_traits>

namespace pure
{
    template<typename T>
    struct Vec2
    {
        static_assert(std::is_fundamental<T>::value, "Type of Vector must be primitive");

        union
        {
            struct { T x, y; };
            struct { T r, g; };
        };



        constexpr Vec2(T x = T(0.0), T y = T(0.0)): x(x), y(y) { }

        Vec2<T> static constexpr single(T val) { return { val, val }; }

        Vec2<T>& operator+=(const Vec2<T>& vec)
        {
            x += vec.x;
            y += vec.y;
            return *this;
        }

        Vec2<T>& operator+=(T scalar)
        {
            x += scalar;
            y += scalar;
            return *this;
        }

        Vec2<T>& operator-=(const Vec2<T>& vec)
        {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }

        Vec2<T>& operator-=(T scalar)
        {
            x -= scalar;
            y -= scalar;
            return *this;
        }

        Vec2<T>& operator*=(const Vec2<T>& vec)
        {
            x *= vec.x;
            y *= vec.y;
            return *this;
        }

        Vec2<T>& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vec2<T>& operator/=(const Vec2<T>& vec)
        {
            x /= vec.x;
            y /= vec.y;
            return *this;
        }

        Vec2<T>& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        static constexpr Vec2<T> EMPTY()
        {
            return { T(), T() };
        }
    };

    using Vec2f = Vec2<float>;
    using Vec2i = Vec2<int>;
    using Vec2ui = Vec2<uint32_t>;

    template<typename T>
    constexpr Vec2<T> operator-(const Vec2<T>& vec)
    {
        return {
                -vec.x,
                -vec.y
        };
    }

    template<typename T>
    constexpr Vec2<T> operator+(const Vec2<T>& vec, T scalar)
    {
        return {
                vec.x + scalar,
                vec.y + scalar
        };
    }

    template<typename T>
    constexpr Vec2<T> operator+(T scalar, const Vec2<T>& vec)
    {
        return {
                scalar + vec.x,
                scalar + vec.y
        };
    }

    template<typename T>
    constexpr Vec2<T> operator+(const Vec2<T>& left, const Vec2<T>& right)
    {
        return {
                left.x + right.x,
                left.y + right.y
        };
    }


    template <typename T>
    constexpr Vec2<T> operator-(const Vec2<T>& vec, T scalar)
    {
        return {
                vec.x - scalar,
                vec.y - scalar
        };
    }

    template <typename T>
    constexpr Vec2<T> operator-(T scalar, const Vec2<T>& vec)
    {
        return {
                scalar - vec.x,
                scalar - vec.y
        };
    }

    template <typename T>
    constexpr Vec2<T> operator-(const Vec2<T>& left, const Vec2<T>& right)
    {
        return {
                left.x - right.x,
                left.y - right.y
        };
    }

    template <typename T>
    constexpr Vec2<T> operator*(const Vec2<T>& vec, T scalar)
    {
        return {
                vec.x * scalar,
                vec.y * scalar
        };
    }

    template <typename T>
    constexpr Vec2<T> operator*(T scalar, const Vec2<T>& vec)
    {
        return {
                scalar * vec.x,
                scalar * vec.y
        };
    }

    template <typename T>
    constexpr Vec2<T> operator*(const Vec2<T>& left, const Vec2<T>& right)
    {
        return {
                left.x * right.x,
                left.y * right.y
        };
    }

    template <typename T>
    constexpr Vec2<T> operator/(const Vec2<T>& vec, T scalar)
    {
        return {
                vec.x / scalar,
                vec.y / scalar
        };
    }

    template <typename T>
    constexpr Vec2<T> operator/(T scalar, const Vec2<T>& vec)
    {
        return {
                scalar / vec.x,
                scalar / vec.y
        };
    }

    template <typename T>
    constexpr Vec2<T> operator/(const Vec2<T>& left, const Vec2<T>& right)
    {
        return {
                left.x / right.x,
                left.y / right.y
        };
    }

    template <typename T>
    constexpr bool operator==(const Vec2<T>& left, const Vec2<T>& right)
    {
        return (left.x == right.x) && (left.y == right.y);
    }

    template <typename T>
    constexpr bool operator!=(const Vec2<T>& left, const Vec2<T>& right)
    {
        return !(left == right);
    }
}

#endif //PURE2D_VEC2_H
