//
// Created by matt on 9/3/18.
//
#include "Renderer.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderable.h"
#include "Graphics/Mesh.h"
#include "Window/Window.h"
#include "Private/Shaders.h"
#include "Private/Util.h"
#include <string>
#include <vector>
#include "glad.h"
#include "Quad.h"
#include <iostream>

using namespace pure;

#define GET_MEM_OFFSET(t, member) (&(((t*)0)->member))

static constexpr float CLIP_NEAR = -50.f;
static constexpr float CLIP_FAR = 50.f;

static struct
{
    VertexAttribute vertex2dAttribs[3] = {};
    VertexAttribute instancedMatAttribs [8] = {};
} attribs = {};

static const Quad defaultQuad = Quad::create();

void pure::Renderer::create()
{
	constexpr int NUM_MAT4 = 2;
	{
		cam.position.z = -5.f;
		cam.lookDir.z = -1.f;
	}

	{

		attribs.vertex2dAttribs[0] = VertexAttribute::create(0, 3, DataType::FLOAT, sizeof(Vertex2D), 0, false);
		attribs.vertex2dAttribs[1] = VertexAttribute::create(1, 2, DataType::FLOAT, sizeof(Vertex2D), size_t(GET_MEM_OFFSET(Vertex2D, texCoord)), false);
		attribs.vertex2dAttribs[2] = VertexAttribute::create(2, 4, DataType::FLOAT, sizeof(Vertex2D), size_t(GET_MEM_OFFSET(Vertex2D, color)), false);

		for (int i = 0; i < 4 * NUM_MAT4; i++)
			attribs.instancedMatAttribs[i] = VertexAttribute::create(i + 3, 4, DataType::FLOAT, sizeof(Mat4) * NUM_MAT4, size_t(i * sizeof(Vec4f)), true);
	}

	{
		m_shader = Shader::createSrc(shader::vert, shader::frag);
		m_instancedShader = Shader::createSrc(shader::instancedVert, shader::frag);
		m_defaultTexture = Texture::createBlank();
	}

	{

		Quad q = Quad::create();

		m_quadVAO = VertexArray::create();
		m_quadVAO.bind();

		m_quadBuffer = VertexBuffer::create(q.verts, Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW);

		setVertexLayout(m_quadBuffer, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

		unbindVAO();
	}

	{
		m_drawVAO = VertexArray::create();
		m_drawVAO.bind();

		m_instancedMatBuffer = VertexBuffer::createZeroed(sizeof(Mat4) * NUM_MAT4, 20, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);

		unbindVAO();
	}

}

void pure::Renderer::create(const Rectf & viewport)
{
	create();
	setViewport(viewport);
}

const Rectf & pure::Renderer::viewport() const { return m_viewport; }
const Mat4 &Renderer::projection() const { return m_projection; }

const Mat4 Renderer::MVMatrix() const { return m_projection * cam.view(); }

void Renderer::drawQuad(const Quad &quad, const Mat4 *transform, const Texture *texture)
{
    drawQuad(quad, transform, m_shader, texture);
}


void Renderer::drawQuad(const Quad &quad, const Mat4 *transform, Shader shader, const Texture *texture)
{
    m_quadVAO.bind();
    shader.bind();
    if (texture) texture->bind();
    else m_defaultTexture.bind();

    if (transform)
    {
        shader.setUniform(shader.modelMatLoc_, *transform);
        shader.setUniform(shader.mvpMatLoc_, m_projection * cam.view() * (*transform));
    }
    else
    {
        const Mat4 mat = makeMat4();
        shader.setUniform(shader.modelMatLoc_, mat);
        shader.setUniform(shader.mvpMatLoc_, mat);
    }

    m_quadBuffer.writeBuffer(quad.verts, Quad::VERT_COUNT, 0);


    drawArrays(DrawPrimitive::TRIANGLE_STRIP, 0, m_quadBuffer.vertCount);
}


void pure::Renderer::setViewport(const Rectf& vp)
{
    m_projection = orthographic(vp.x, vp.w, vp.h, vp.y, CLIP_NEAR, CLIP_FAR);
    m_viewport = vp;
}

// TODO: This doesn't kind of zooms from the top left corner, make this zoom from center.
void pure::Renderer::zoom(float offset)
{
    m_viewport.w += offset;
    m_viewport.h += offset;
    m_projection = orthographic(m_viewport.x,
                                m_viewport.w, m_viewport.h,
                                m_viewport.y, CLIP_NEAR, CLIP_FAR
    );

}

void pure::Renderer::drawMesh(const Mesh & mesh, const Mat4 & transform)
{
    m_drawVAO.bind();

    const Shader& shader = mesh.shader.id_ == 0 ? m_shader : mesh.shader;
    shader.bind();

    if (mesh.texture) mesh.texture->bind();
    else m_defaultTexture.bind();

    mesh.vbo.bind();

    setVertexLayout(mesh.vbo, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    shader.setUniform(shader.mvpMatLoc_, m_projection * cam.view() * transform);
    shader.setUniform(shader.modelMatLoc_, transform);

	if (mesh.ebo.id_ != 0)
	{
		mesh.ebo.bind();
		drawElements(mesh.primtype, mesh.ebo.count);
		unbindEBO();
	}
	else
	{
		drawArrays(mesh.primtype, 0, uint32_t(mesh.vbo.vertCount));
	}

    unbindVBO();
}

void pure::Renderer::drawMeshStatic(const Mesh & mesh)
{
	drawBuffer(0, mesh.vbo.vertCount, mesh.vbo, mesh.texture, mesh.shader, 
		(mesh.ebo.id_ ? &mesh.ebo : nullptr), mesh.primtype);
}

void pure::Renderer::drawMeshInstanced(const Mesh & mesh, const Mat4 * transforms, uint32_t numDraws)
{
    m_drawVAO.bind();
    if (mesh.shader.id_ == 0) m_instancedShader.bind();
    else mesh.shader.bind();

    if (mesh.texture) mesh.texture->bind();
    else m_defaultTexture.bind();

    setVertexLayout(mesh.vbo, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    std::vector<Mat4> transformData;
    transformData.reserve(numDraws);

    for (size_t i = 0; i < numDraws; i++)
    {
        const Mat4 &model = transforms[i];
        transformData.push_back(m_projection * cam.view() * model);
        transformData.push_back(model);
    }

    setVertexLayout(m_instancedMatBuffer, attribs.instancedMatAttribs, ARRAY_COUNT(attribs.instancedMatAttribs));

    if (transformData.size() * sizeof(Mat4) >= m_instancedMatBuffer.size)
        m_instancedMatBuffer.alloc(&transformData[0], transformData.size(), DrawUsage::DYNAMIC_DRAW);
    else
        m_instancedMatBuffer.writeBuffer(&transformData[0], transformData.size(), 0);

    glDrawArraysInstanced(static_cast<GLenum>(mesh.primtype), 0, uint32_t(mesh.vbo.vertCount), numDraws);
    unbindVBO();
}

void Renderer::drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture, const ElementBuffer* ebo, DrawPrimitive primtype)
{
    drawBuffer(start, count, buffer, texture, m_shader, ebo, primtype);
}

void Renderer::drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture, Shader shader, const ElementBuffer* ebo, DrawPrimitive primtype)
{
    m_drawVAO.bind();
    shader.bind();
    if (texture) texture->bind();
    else m_defaultTexture.bind();

	buffer.bind();

    // TODO: Maybe we can use a different simple shader to avoid sending useless data to gpu?
    shader.setUniform(shader.modelMatLoc_, makeMat4());
    shader.setUniform(shader.mvpMatLoc_, makeMat4());

    setVertexLayout(buffer, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    if (ebo)
    {
		ebo->bind();
        drawElements(primtype, ebo->count);
		unbindEBO();
    }
    else
    {
        drawArrays(primtype, start, count);
    }

    unbindVBO();
}

void Renderer::draw(Renderable &renderable)
{
    renderable.draw(*this);
}

void Renderer::destroy()
{
    m_quadVAO.free();
    m_quadBuffer.free();
    m_instancedMatBuffer.free();

    m_instancedShader.free();
    m_shader.free();
    m_defaultTexture.free();

    m_drawVAO.free();
}

