//
// Created by matt on 9/3/18.
//
#include "Mat4.h"
#include "Private/Convert.h"
#include "Math/Vec4.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <new>

using namespace pure;

Mat4 pure::Mat4::identity()
{
	glm::vec4 v;
	Mat4 m = {};
	new (&m.val_) glm::mat4(1);
	return m;
}

Mat4 pure::Mat4::make(float m4x4[16])
{
	Mat4 m = {};
	new (&m.val_) glm::mat4(
		m4x4[0], m4x4[4], m4x4[8],  m4x4[12],
		m4x4[1], m4x4[5], m4x4[8],  m4x4[13],
		m4x4[2], m4x4[6], m4x4[10], m4x4[14],
		m4x4[3], m4x4[7], m4x4[11], m4x4[15]
	);
}

Mat4 pure::operator-(const Mat4 & operand)
{
    return toMat4(-GLM_MAT4_CONST(operand));
}

Mat4 pure::operator+(const Mat4 & left, const Mat4 & right)
{
    return toMat4(GLM_MAT4_CONST(left) + GLM_MAT4_CONST(right));
}

Mat4 pure::operator-(const Mat4 & left, const Mat4 & right)
{
    return toMat4(GLM_MAT4_CONST(left) - GLM_MAT4_CONST(right));
}

Mat4 pure::operator*(const Mat4 & left, const Mat4 & right)
{
    return toMat4(GLM_MAT4_CONST(left) * GLM_MAT4_CONST(right));
}

Vec4f pure::operator*(const Vec4<float>& vec, const Mat4 & right)
{
    return toVec4(toGlmVec4(vec) * GLM_MAT4_CONST(right));
}

Vec4f pure::operator*(const Mat4 & left, const Vec4<float>& vec)
{
    return toVec4(GLM_MAT4_CONST(left) * toGlmVec4(vec));
}

Mat4 pure::operator/(const Mat4 & left, const Mat4 & right)
{
    return toMat4(GLM_MAT4_CONST(left) / GLM_MAT4_CONST(right));
}

bool pure::operator==(const Mat4 & left, const Mat4 & right)
{
    return (GLM_MAT4_CONST(left) == GLM_MAT4_CONST(right));
}

bool pure::operator!=(const Mat4 & left, const Mat4 & right)
{
    return (GLM_MAT4_CONST(left) != GLM_MAT4_CONST(right));
}

void pure::Mat4::raw(float out[16]) const
{
    memcpy(out, TO_GLM_MAT4_CONST(val_), 16 * sizeof(float));
}

float * pure::Mat4::ptr()
{
    return glm::value_ptr(GLM_MAT4((*this)));
}

Mat4 & pure::Mat4::operator=(const Mat4 & other)
{
    GLM_MAT4((*this)) = GLM_MAT4_CONST(other);
    return *this;
}

Mat4 & pure::Mat4::operator+=(const Mat4 & other)
{
    GLM_MAT4((*this)) += GLM_MAT4_CONST(other);
    return *this;
}

Mat4 & pure::Mat4::operator-=(const Mat4 & other)
{
    GLM_MAT4((*this)) -= GLM_MAT4_CONST(other);
    return *this;
}

Mat4 & pure::Mat4::operator*=(const Mat4 & other)
{
    GLM_MAT4((*this)) *= GLM_MAT4_CONST(other);
    return *this;
}

Mat4 & pure::Mat4::operator/=(const Mat4 & other)
{
    GLM_MAT4((*this)) /= GLM_MAT4_CONST(other);
    return *this;
}

Mat4 & Mat4::operator+=(float other)
{
    GLM_MAT4((*this)) +=other;
    return *this;
}

Mat4 & Mat4::operator-=(float other)
{
    GLM_MAT4((*this)) -= other;
    return *this;
}

Mat4 & Mat4::operator*=(float other)
{
    GLM_MAT4((*this)) *= other;
    return *this;
}

Mat4 & Mat4::operator/=(float other)
{
    GLM_MAT4((*this)) /= other;
    return *this;
}

Mat4 & pure::Mat4::operator++()
{
    ++(GLM_MAT4((*this)));
    return *this;
}

Mat4 & pure::Mat4::operator--()
{
    --(GLM_MAT4((*this)));
    return *this;
}

Mat4 & pure::Mat4::operator++(int)
{
    (GLM_MAT4((*this)))++;
    return *this;
}

Mat4 & pure::Mat4::operator--(int)
{
    (GLM_MAT4((*this)))--;
    return *this;
}

Mat4 operator+(const Mat4 & left, float right)
{
    return toMat4(GLM_MAT4_CONST(left) + right);
}

Mat4 operator+(float left, const Mat4 & right)
{
    return toMat4(left + GLM_MAT4_CONST(right));
}

Mat4 operator-(const Mat4 & left, float right)
{
    return toMat4(GLM_MAT4_CONST(left) - right);
}

Mat4 operator-(float left, const Mat4 & right)
{
    return toMat4(left - GLM_MAT4_CONST(right));
}

Mat4 operator*(const Mat4 & left, float right)
{
    return toMat4(GLM_MAT4_CONST(left) * right);
}

Mat4 operator*(float left, const Mat4 & right)
{
    return toMat4(left * GLM_MAT4_CONST(right));
}

Mat4 operator/(const Mat4 & left, float right)
{
    return toMat4(GLM_MAT4_CONST(left) / right);
}

Mat4 operator/(float left, const Mat4 & right)
{
    return toMat4(left / GLM_MAT4_CONST(right));
}
