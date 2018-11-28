//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_RENDERER_H
#define PURE2D_RENDERER_H


#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Vec4.h>
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
	struct RenderTexture;
    template <typename T> struct Vec2;
    template <typename T> struct Vec4;
    template <typename T> struct Rect;

    struct Renderer : private NonCopyable
    {
        Camera cam;

		PURE2D_API static float clipNear();
		PURE2D_API static float clipFar();

		PURE2D_API void clear(const Vec4f& color = { 0.f, 0.f, 0.f, 1.f }) const;
		PURE2D_API void present() const;

		PURE2D_API void create(const Window& win);

		PURE2D_API void beginDrawTexture(const RenderTexture& rt);
		PURE2D_API void endDrawTexture();

		PURE2D_API const RenderTexture* drawTarget() const;

		PURE2D_API void drawMesh(const Mesh& mesh, const Mat4& transform);

		// draws mesh without transforming verts by renderer projection and view matrix
		PURE2D_API void drawMeshStatic(const Mesh& mesh);

		PURE2D_API void drawMeshInstanced(const Mesh& mesh, const Mat4* transforms, uint32_t numDraws);

        PURE2D_API void drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture,
                                   Shader shader, const ElementBuffer* ebo = nullptr, DrawPrimitive primtype = DrawPrimitive::TRIANGLE_STRIP);
		PURE2D_API void drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture,
							const ElementBuffer* ebo = nullptr, DrawPrimitive primtype = DrawPrimitive::TRIANGLE_STRIP);

		PURE2D_API void drawQuad(const Quad& quad, const Mat4* transform, const Texture* texture = nullptr);
        PURE2D_API void drawQuad(const Quad& quad, const Mat4* transform, Shader shader, const Texture* texture = nullptr);

        PURE2D_API void draw(Renderable& renderable);

        PURE2D_API const Mat4& projection() const;
        PURE2D_API Mat4 MVMatrix() const;
		// Gets the cached glViewport Rect
        PURE2D_API const Rectf& viewport() const;
		// Set the cached glViewport Rect
        PURE2D_API void setViewport(const Rectf& vp);
        PURE2D_API void zoom(float offset);

        PURE2D_API void destroy();

		// Binds internal VAO if not already bound in case
		// you want to use external VAO for whatever reason
		PURE2D_API void activate();

    private:
		float m_clipNear, m_clipFar;

        VertexArray m_drawVAO;
        VertexBuffer m_quadBuffer;
        VertexBuffer m_instancedMatBuffer;

        Shader m_shader;
        Shader m_instancedShader;

        Texture m_defaultTexture;
        Mat4 m_projection;
        Rectf m_viewport;

		const Window* m_targetWindow;

		// NOTE: Do we even need this? guess its cool if we want to check 
		// if and what we have as active draw target.
		const RenderTexture* m_userDrawTarget;
    };
}



#endif //PURE2D_RENDERER_H
