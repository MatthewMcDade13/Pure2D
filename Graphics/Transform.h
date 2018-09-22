//
// Created by matt on 9/16/18.
//

#ifndef PURE2D_TRANSFORM_H
#define PURE2D_TRANSFORM_H

#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Mat4.h"
#include "Define.h"

namespace pure
{
    struct Transform
    {

        PURE2D_API static Transform create(Mat4 mat = makeMat4());
        PURE2D_API Vec3f position() const;
        PURE2D_API Vec2f size() const;
        PURE2D_API float rotation() const;
        PURE2D_API void setPosition(const Vec3f& pos);
        PURE2D_API void setSize(Vec2f size);
        PURE2D_API void setRotation(float angle);
        PURE2D_API const Mat4& modelMatrix();

        PURE2D_API void move(const Vec3f& offset);
        PURE2D_API void scale(Vec2f offset);
        PURE2D_API void rotate(float angle);

    private:
        Vec3f m_position;
        Vec2f m_size;
        float m_rotation;
        Mat4 m_model;

        bool m_needsUpdate;
    };
}


#endif //PURE2D_TRANSFORM_H
