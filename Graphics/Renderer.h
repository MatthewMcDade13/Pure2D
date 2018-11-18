//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_RENDERER_H
#define PURE2D_RENDERER_H


#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Mat4.h>
#include <Pure2D/Math/Rect.h>
#include <Pure2D/Graphics/Shader.h>
#include <Pure2D/Graphics/Vertex.h>
#include <Pure2D/Graphics/Buffers.h>
#include <Pure2D/Graphics/Camera.h>
#include <Pure2D/Graphics/Transform.h>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/System/NonCopyable.h>
#include <Pure2D/Define.h>

namespace pure
{
    struct Window;
    struct Sprite;
	struct Mesh;
	struct Renderable;
	struct Quad;
    template <typename T> struct Vec2;
    template <typename T> struct Vec4;
    template <typename T> struct Rect;

    struct Renderer : private NonCopyable
    {
        Camera cam;

		PURE2D_API static float clipNear();
		PURE2D_API static float clipFar();

		PURE2D_API void create();
		PURE2D_API void create(const Rectf& viewport);

		PURE2D_API void drawMesh(const Mesh& m_mesh, const Mat4& transform);

		// draws mesh without transforming verts by renderer projection and view matrix
		PURE2D_API void drawMeshStatic(const Mesh& m_mesh);

		PURE2D_API void drawMeshInstanced(const Mesh& m_mesh, const Mat4* transforms, uint32_t numDraws);

        PURE2D_API void drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture,
                                   Shader shader, const ElementBuffer* ebo = nullptr, DrawPrimitive primtype = DrawPrimitive::TRIANGLE_STRIP);
		PURE2D_API void drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture,
							const ElementBuffer* ebo = nullptr, DrawPrimitive primtype = DrawPrimitive::TRIANGLE_STRIP);

		PURE2D_API void drawQuad(const Quad& quad, const Mat4* transform, const Texture* texture = nullptr);
        PURE2D_API void drawQuad(const Quad& quad, const Mat4* transform, Shader shader, const Texture* texture = nullptr);

        PURE2D_API void draw(Renderable& renderable);

        PURE2D_API const Mat4& projection() const;
        PURE2D_API const Mat4 MVMatrix() const;
		// Gets the cached glViewport Rect
        PURE2D_API const Rectf& viewport() const;
		// Set the cached glViewport Rect
        PURE2D_API void setViewport(const Rectf& vp);
        PURE2D_API void zoom(float offset);

        PURE2D_API void destroy();

    private:
		float m_clipNear, m_clipFar;

        VertexArray m_quadVAO;
        VertexBuffer m_quadBuffer;
        VertexBuffer m_instancedMatBuffer;
        VertexArray m_drawVAO;

        Shader m_shader;
        Shader m_instancedShader;

        Texture m_defaultTexture;
        Mat4 m_projection;
        Rectf m_viewport;
    };
}



#endif //PURE2D_RENDERER_H
