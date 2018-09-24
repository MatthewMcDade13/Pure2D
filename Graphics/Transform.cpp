//
// Created by matt on 9/16/18.
//

#include "Transform.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"


using namespace pure;

Vec3f pure::Transform::position() const { return m_position; }
Vec2f pure::Transform::size() const { return m_size; }

float pure::Transform::rotation() const { return m_rotation; }

void pure::Transform::setPosition(const pure::Vec3f& pos)
{
    m_position = pos;
    m_needsUpdate = true;
}

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
        m_model = translate(makeMat4(), m_position);
        m_model = translate(m_model, Vec3f(m_size.x * .5f, m_size.y * .5f, 0.f));
        m_model = pure::rotate(m_model, radians(m_rotation), Vec3f(0.f, 0.f, 1.f));
        m_model = translate(m_model, Vec3f(m_size.x * -.5f, m_size.y * -.5f, 0.f));
        m_model = pure::scale(m_model, Vec3f(m_size.x, m_size.y, 1.f));
        m_needsUpdate = false;
    }

    return m_model;
}

void Transform::move(const Vec3f& offset)
{
    m_position += offset;
    m_needsUpdate = true;
}

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

Transform Transform::create(Mat4 mat)
{
	Transform t = {};
    t.m_model = mat;
    t.m_needsUpdate = true;
	return t;
}
