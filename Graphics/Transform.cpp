//
// Created by matt on 9/16/18.
//

#include "Transform.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"


using namespace pure;

const Vec3f& pure::Transform::position() const { return m_position; }
Vec2f pure::Transform::size() const { return m_size; }

float pure::Transform::rotation() const { return m_rotation; }

void Transform::setPosition(float x, float y) { setPosition(Vec2f(x, y)); }
void Transform::setPosition(float x, float y, float z) { setPosition(Vec3f(x, y, z)); }
void pure::Transform::setPosition(Vec2f pos) { setPosition(Vec3f(pos.x, pos.y, m_position.z)); }
void pure::Transform::setPosition(const pure::Vec3f& pos)
{
    m_position = pos;
    m_needsUpdate = true;
}

void Transform::setSize(float w, float h) { setSize(Vec2f(w, h)); }
void pure::Transform::setSize(pure::Vec2f size)
{
    m_size = size;
    m_needsUpdate = true;
}

void pure::Transform::setRotation(float angle)
{
    m_rotation = angle;
    m_needsUpdate = true;
}

const pure::Mat4 &pure::Transform::modelMatrix()
{
    if (m_needsUpdate)
    {
        m_model = translate(Mat4::identity(), m_position);
        m_model = translate(m_model, Vec3f(m_size.x * .5f, m_size.y * .5f, 0.f));
        m_model = pure::rotate(m_model, radians(m_rotation), Vec3f(0.f, 0.f, 1.f));
        m_model = translate(m_model, Vec3f(m_size.x * -.5f, m_size.y * -.5f, 0.f));
        m_model = pure::scale(m_model, Vec3f(m_size.x, m_size.y, 1.f));
        m_needsUpdate = false;
    }

    return m_model;
}

void Transform::move(float x, float y) { move(Vec2f(x, y)); }
void Transform::move(float x, float y, float z) { move(Vec3f(x, y, z)); }
void Transform::move(Vec2f offset) { move({ offset.x, offset.y, 0.f }); }
void Transform::move(const Vec3f& offset)
{
    m_position += offset;
    m_needsUpdate = true;
}

void Transform::scale(float x, float y) { scale(Vec2f(x, y)); }
void Transform::scale(Vec2f offset)
{
    m_size += offset;
    m_needsUpdate = true;
}

void Transform::rotate(float angle)
{
    m_rotation += angle;
    m_needsUpdate = true;
}

Transform Transform::make(Mat4 mat)
{
	Transform t = {};
    t.m_model = mat;
    t.m_needsUpdate = true;
	return t;
}
