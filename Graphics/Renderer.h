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
#include "Graphics/Transform.h"
#include "System/NonCopyable.h"
#include "Define.h"
#include "Texture.h"

namespace pure
{
    struct Window;
    struct Sprite;
	struct Mesh;
	struct Renderable;
    template <typename T> struct Vec2;
    template <typename T> struct Vec4;
    template <typename T> struct Rect;

    struct Renderer : private NonCopyable
    {
        Camera cam;

        PURE2D_API explicit Renderer(const Window& window);
        PURE2D_API ~Renderer();

		PURE2D_API void drawMesh(const Mesh& mesh, const Mat4& transform);
		PURE2D_API void drawMeshInstanced(const Mesh& mesh, const Mat4* transforms, uint32_t numDraws);

		// TODO: Change to drawQuads when we implement Element Buffers... or some ish lol
        PURE2D_API void drawTriangles(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture,
                                      Shader shader);
		PURE2D_API void drawTriangles(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture* texture);

		// TODO: Verify these drawTexture funcs actually work still after we refactored Renderer.
        PURE2D_API void drawTexture(const Texture& tex, Vec3f pos, Vec2f size, float rotation = 0, const Rectui* texRect = nullptr) const;
        PURE2D_API void drawTexture(const Texture& tex, Vec3f pos, Vec2f size, Shader shader, float rotation = 0, const Rectui* texRect = nullptr) const;

        PURE2D_API void drawTexture(const Texture& tex, const Mat4& transform) const;
        PURE2D_API void drawTexture(const Texture& tex, const Mat4& transform, Shader shader) const;

        PURE2D_API void draw(Renderable& renderable);

        PURE2D_API const Mat4& projection() const;
        PURE2D_API const Rectf& viewport() const;
        PURE2D_API void setViewport(const Rectf& vp);
        PURE2D_API void zoom(float offset);

    private:
        VertexArray m_vao;
        VertexBuffer m_vertBuffer;
        VertexBuffer m_instancedMatBuffer;
        VertexArray m_drawVAO;
        VertexBuffer m_drawBuff;

        Shader m_shader;
        Shader m_instancedShader;
        Texture m_defaultTexture;
        Mat4 m_projection;
        Rectf m_viewport;
    };
}



#endif //PURE2D_RENDERER_H
