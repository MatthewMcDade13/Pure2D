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

#ifndef EXPIRIMENTAL_MAT4

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
		m4x4[0], m4x4[1], m4x4[2],  m4x4[3],
		m4x4[4], m4x4[5], m4x4[6],  m4x4[7],
		m4x4[8], m4x4[9], m4x4[10], m4x4[11],
		m4x4[12], m4x4[13], m4x4[14], m4x4[15]
	);

	return m;
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
	const float* a = left.cptr();
	const float* b = right.cptr();

	float result[16] = {};

	result[0] = b[0]*a[0] + b[1]*a[4] + b[2]*a[8]  + b[3]*a[12];
	result[1] = b[0]*a[1] + b[1]*a[5] + b[2]*a[9]  + b[3]*a[13];
	result[2] = b[0]*a[2] + b[2]*a[6] + b[2]*a[10] + b[3]*a[14];
	result[3] = b[0]*a[3] + b[3]*a[7] + b[2]*a[11] + b[3]*a[15];

	result[4] = b[4]*a[0] + b[5]*a[4] + b[6]*a[8]  + b[7]*a[12];
	result[5] = b[4]*a[1] + b[5]*a[5] + b[6]*a[9]  + b[7]*a[13];
	result[6] = b[4]*a[2] + b[5]*a[6] + b[6]*a[10] + b[7]*a[14];
	result[7] = b[4]*a[3] + b[5]*a[7] + b[6]*a[11] + b[7]*a[15];

	result[8]  = b[8]*a[0] + b[9]*a[4] + b[10]*a[8]  + b[11]*a[12];
	result[9]  = b[8]*a[1] + b[9]*a[5] + b[10]*a[9]  + b[11]*a[13];
	result[10] = b[8]*a[2] + b[9]*a[6] + b[10]*a[10] + b[11]*a[14];
	result[11] = b[8]*a[3] + b[9]*a[7] + b[10]*a[11] + b[11]*a[15];

	result[12] = b[12]*a[0] + b[13]*a[4] + b[14]*a[8]  + b[15]*a[12];
	result[13] = b[12]*a[1] + b[13]*a[5] + b[14]*a[9]  + b[15]*a[13];
	result[14] = b[12]*a[2] + b[13]*a[6] + b[14]*a[10] + b[15]*a[14];
	result[15] = b[12]*a[3] + b[13]*a[7] + b[14]*a[11] + b[15]*a[15];

	return Mat4::make(result);
    //return toMat4(GLM_MAT4_CONST(left) * GLM_MAT4_CONST(right));
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

const float * pure::Mat4::cptr() const
{
	return glm::value_ptr(GLM_MAT4_CONST((*this)));
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

#else
//
// Created by matt on 9/3/18.
//
#include "Mat4.h"
#include "Private/Convert.h"
#include <Pure2D/Math/Manip.h>
#include "Math/Vec4.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <new>

using namespace pure;

Mat4 pure::Mat4::identity()
{
	Mat4 m;
	m.val.col[0] = { 1.f, 0.f, 0.f, 0.f };
	m.val.col[1] = { 0.f, 1.f, 0.f, 0.f };
	m.val.col[2] = { 0.f, 0.f, 1.f, 0.f };
	m.val.col[3] = { 0.f, 0.f, 0.f, 1.f };

	return m;
}

Mat4 pure::Mat4::make(const float m4x4[16])
{
	Mat4 m = {};

	memcpy(m.val.buf, m4x4, sizeof(float) * 16);

	return m;
}

Mat4 pure::operator-(const Mat4 & operand)
{
	Mat4 result = operand;
	for (size_t i = 0; i < 4; i++)
		result.val.col[i] = -result.val.col[i];
	return result;
}

Mat4 pure::operator+(const Mat4 & left, const Mat4 & right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left.val.col[i] + right.val.col[i];
	return m;
}

Mat4 pure::operator-(const Mat4 & left, const Mat4 & right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left.val.col[i] - right.val.col[i];
	return m;
}

Mat4 pure::operator*(const Mat4 & left, const Mat4 & right)
{
	Mat4 m;
	m.val.col[0].x = dot(left.row(0), right.val.col[0]);
	m.val.col[0].y = dot(left.row(1), right.val.col[0]);
	m.val.col[0].z = dot(left.row(2), right.val.col[0]);
	m.val.col[0].w = dot(left.row(3), right.val.col[0]);

	m.val.col[1].x = dot(left.row(0), right.val.col[1]);
	m.val.col[1].y = dot(left.row(1), right.val.col[1]);
	m.val.col[1].z = dot(left.row(2), right.val.col[1]);
	m.val.col[1].w = dot(left.row(3), right.val.col[1]);

	m.val.col[2].x = dot(left.row(0), right.val.col[2]);
	m.val.col[2].y = dot(left.row(1), right.val.col[2]);
	m.val.col[2].z = dot(left.row(2), right.val.col[2]);
	m.val.col[2].w = dot(left.row(3), right.val.col[2]);

	m.val.col[3].x = dot(left.row(0), right.val.col[3]);
	m.val.col[3].y = dot(left.row(1), right.val.col[3]);
	m.val.col[3].z = dot(left.row(2), right.val.col[3]);
	m.val.col[3].w = dot(left.row(3), right.val.col[3]);

	return m;
}

Vec4f pure::operator*(const Vec4<float>& vec, const Mat4 & right)
{
	const float* mb = right.val.buf;
	return {
		dot(vec, right.row(0)),
		dot(vec, right.row(1)),
		dot(vec, right.row(2)),
		dot(vec, right.row(3)),
	};
}

Vec4f pure::operator*(const Mat4 & left, const Vec4<float>& vec)
{
	return vec * left;
}

bool pure::operator==(const Mat4 & left, const Mat4 & right)
{
	return (left.val.col[0] == right.val.col[0])
		&& (left.val.col[1] == right.val.col[1])
		&& (left.val.col[2] == right.val.col[2])
		&& (left.val.col[3] == right.val.col[3]);
}

bool pure::operator!=(const Mat4 & left, const Mat4 & right)
{
	return !(left == right);
}

Mat4 & pure::Mat4::operator=(const Mat4 & other)
{
	memcpy(val.buf, other.val.buf, sizeof(float) * 16);
	return *this;
}

Mat4 & pure::Mat4::operator+=(const Mat4 & other)
{
	for (size_t i = 0; i < 4; i++)
		val.col[i] += other.val.col[i];
	return *this;
}

Mat4 & pure::Mat4::operator-=(const Mat4 & other)
{
	for (size_t i = 0; i < 4; i++)
		val.col[i] -= other.val.col[i];
	return *this;
}

Mat4 & pure::Mat4::operator*=(const Mat4 & other)
{
	val.col[0].x = dot(row(0), other.val.col[0]);
	val.col[0].y = dot(row(1), other.val.col[0]);
	val.col[0].z = dot(row(2), other.val.col[0]);
	val.col[0].w = dot(row(3), other.val.col[0]);

	val.col[1].x = dot(row(0), other.val.col[1]);
	val.col[1].y = dot(row(1), other.val.col[1]);
	val.col[1].z = dot(row(2), other.val.col[1]);
	val.col[1].w = dot(row(3), other.val.col[1]);

	val.col[2].x = dot(row(0), other.val.col[2]);
	val.col[2].y = dot(row(1), other.val.col[2]);
	val.col[2].z = dot(row(2), other.val.col[2]);
	val.col[2].w = dot(row(3), other.val.col[2]);

	val.col[3].x = dot(row(0), other.val.col[3]);
	val.col[3].y = dot(row(1), other.val.col[3]);
	val.col[3].z = dot(row(2), other.val.col[3]);
	val.col[3].w = dot(row(3), other.val.col[3]);
	return *this;
}

Mat4 & Mat4::operator+=(float other)
{
	for (size_t i = 0; i < 4; i++)
		val.col[i] += other;
	return *this;
}

Mat4 & Mat4::operator-=(float other)
{
	for (size_t i = 0; i < 4; i++)
		val.col[i] -= other;
	return *this;
}

Mat4 & Mat4::operator*=(float other)
{
	for (size_t i = 0; i < 4; i++)
		val.col[i] *= other;
	return *this;
}

Mat4 operator+(const Mat4 & left, float right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left.val.col[i] + right;
	return m;
}

Mat4 operator+(float left, const Mat4 & right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left + right.val.col[i];
	return m;
}

Mat4 operator-(const Mat4 & left, float right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left.val.col[i] - right;
	return m;
}

Mat4 operator-(float left, const Mat4 & right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left - right.val.col[i];
	return m;
}

Mat4 operator*(const Mat4 & left, float right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left.val.col[i] * right;
	return m;
}

Mat4 operator*(float left, const Mat4 & right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left * right.val.col[i];
	return m;
}

Mat4 operator/(const Mat4 & left, float right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left.val.col[i] / right;
	return m;
}

Mat4 operator/(float left, const Mat4 & right)
{
	Mat4 m;
	for (size_t i = 0; i < 4; i++)
		m.val.col[i] = left / right.val.col[i];
	return m;
}
#endif