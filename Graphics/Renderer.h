//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_RENDERER_H
#define PURE2D_RENDERER_H


#include "Math/Vec2.h"
#include "Math/Mat4.h"
#include "Math/Rect.h"
#include "Graphics/Shader.h"
#include "Graphics/Vertex.h"
#include "Graphics/Buffers.h"
#include "Graphics/Camera.h"
#include "System/NonCopyable.h"
#include "Define.h"

namespace pure
{
    struct Texture;
    struct Window;
    struct Sprite;
    template <typename T> struct Vec2;
    template <typename T> struct Vec4;
    template <typename T> struct Rect;

    struct Renderer : private NonCopyable
    {
        Camera cam;

        PURE2D_API explicit Renderer(const Window& window);
        PURE2D_API ~Renderer();



        PURE2D_API void drawRect(const Rectf& r, const Vec4<float>& color, float rotation = 0) const;
        PURE2D_API void drawRect(const Rectf& r, const Vec4<float>& color, Shader shader, float rotation = 0) const;

        PURE2D_API void drawPrimitive(DrawPrimitive primtype, const Vertex2D* verts, size_t vertCount);
        PURE2D_API void drawPrimitive(DrawPrimitive primtype, const Vertex2D* verts, size_t vertCount, Shader shader);

        PURE2D_API void drawTexture(const Texture& tex, Vec3f pos, Vec2f size, float rotation = 0, const Rectui* texRect = nullptr) const;
        PURE2D_API void drawTexture(const Texture& tex, Vec3f pos, Vec2f size, Shader shader, float rotation = 0, const Rectui* texRect = nullptr) const;

        PURE2D_API void drawTexture(const Texture& tex, const Mat4& transform, const Rectui* texRect = nullptr) const;
        PURE2D_API void drawTexture(const Texture& tex, const Mat4& transform, Shader shader, const Rectui* texRect = nullptr) const;

        PURE2D_API void drawTextureInstanced(const Texture& tex, const Mat4* transforms, const Rectui* texRects, size_t count);
        PURE2D_API void drawTextureInstanced(const Texture& tex, const Mat4* transforms, const Rectui* texRects, size_t count, Shader shader);

        PURE2D_API void drawSprite(Sprite& sprite) const;
        PURE2D_API void drawSprite(Sprite& sprite, Shader shader) const;

        PURE2D_API void drawSpritesInstanced(Sprite* sprites, size_t count);
        PURE2D_API void drawSpritesInstanced(Sprite* sprites, size_t count, Shader shader);

        PURE2D_API const Rectf& viewport() const;
        PURE2D_API void setViewport(const Rectf& vp);
        PURE2D_API void zoom(float offset);

    private:
        VertexArray m_vao;
        VertexBuffer m_vertBuffer;
        VertexBuffer m_instancedMatBuffer;
        VertexBuffer m_instancedOffsetsBuffer;

        VertexArray m_primitiveVAO;
        VertexBuffer m_primitiveBuffer;

        Shader m_spriteShader;
        Shader m_colorShader;
        Shader m_instancedShader;
        Shader m_basicShader;
        Mat4 m_projection;
        Rectf m_viewport;
    };
}



#endif //PURE2D_RENDERER_H
