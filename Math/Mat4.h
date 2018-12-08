//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_MAT4_H
#define PURE2D_MAT4_H


#include <type_traits>
#include <Pure2D/Define.h>
#include <Pure2D/Graphics/Vertex.h>


namespace pure
{
    // TODO: Add more overloads for vector operations

    template<typename T> struct Vec4;

    struct PURE2D_API Mat4
    {
        // TODO: Might need copy ctor/copyData assignment operator?
        std::aligned_storage_t<sizeof(float[4][4])> val_;
		static Mat4 identity();
		static Mat4 make(float m4x4[16]);

        void raw(float out[16]) const;
        float* ptr();

        Mat4& operator=(const Mat4& other);

        Mat4& operator+=(float scalar);
        Mat4& operator+=(const Mat4& other);

        Mat4& operator-=(float scalar);
        Mat4& operator-=(const Mat4& other);

        Mat4& operator*=(float scalar);
        Mat4& operator*=(const Mat4& other);

        Mat4& operator/=(float scalar);
        Mat4& operator/=(const Mat4& other);

        Mat4& operator++();
        Mat4& operator--();
        Mat4& operator++(int);
        Mat4& operator--(int);

    };


    PURE2D_API Mat4 operator-(const Mat4& operand);

    PURE2D_API Mat4 operator+(const Mat4& left, float right);

    PURE2D_API Mat4 operator+(float left, const Mat4& right);
    PURE2D_API Mat4 operator+(const Mat4& left, const Mat4& right);

    PURE2D_API Mat4 operator-(const Mat4& left, float right);

    PURE2D_API Mat4 operator-(float left, const Mat4& right);
    PURE2D_API Mat4 operator-(const Mat4& left, const Mat4& right);


    PURE2D_API Mat4 operator*(const Mat4& left, float right);

    PURE2D_API Mat4 operator*(float left, const Mat4& right);
    PURE2D_API Mat4 operator*(const Mat4& left, const Mat4& right);
    PURE2D_API Vec4<float> operator*(const Vec4<float>& vec, const Mat4& right);
    PURE2D_API Vec4<float> operator*(const Mat4& left, const Vec4<float>& vec);

    PURE2D_API Mat4 operator/(const Mat4& left, float right);
    PURE2D_API Mat4 operator/(float left, const Mat4& right);
    PURE2D_API Mat4 operator/(const Mat4& left, const Mat4& right);

    PURE2D_API bool operator==(const Mat4& left, const Mat4& right);
    PURE2D_API bool operator!=(const Mat4& left, const Mat4& right);
}




#endif //PURE2D_MAT4_H
