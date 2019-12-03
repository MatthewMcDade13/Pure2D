//
// Created by matt on 9/16/18.
//

#ifndef PURE2D_TRANSFORM_H
#define PURE2D_TRANSFORM_H

#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Vec3.h>
#include <Pure2D/Math/Mat4.h>
#include <Pure2D/Define.h>

namespace pure
{
    struct Transform
    {

        PURE2D_API static Transform make(Mat4 mat = Mat4::identity());
        PURE2D_API const Vec3f& position() const;
        PURE2D_API Vec2f size() const;
        PURE2D_API float rotation() const;

        PURE2D_API void setPosition(float x, float y);
        PURE2D_API void setPosition(float x, float y, float z);
		PURE2D_API void setPosition(Vec2f pos);
        PURE2D_API void setPosition(const Vec3f& pos);

        PURE2D_API void setSize(float w, float h);
        PURE2D_API void setSize(Vec2f size);

        PURE2D_API void setRotation(float angle);

        PURE2D_API const Mat4& modelMatrix();

        PURE2D_API void move(float x, float y);
        PURE2D_API void move(float x, float y, float z);
        PURE2D_API void move(const Vec3f& offset);
        PURE2D_API void move(Vec2f offset);

        PURE2D_API void scale(float x, float y);
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
