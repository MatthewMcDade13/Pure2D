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

static const Quad defaultQuad = Quad::make();

void pure::Renderer::make()
{
	constexpr int NUM_MAT4 = 2;
	{
		cam.position.z = 0.f;
		cam.lookDir.z = -1.f;
	}

	{

		attribs.vertex2dAttribs[0] = VertexAttribute::make(0, 3, DataType::FLOAT, sizeof(Vertex2D), 0, false);
		attribs.vertex2dAttribs[1] = VertexAttribute::make(1, 2, DataType::FLOAT, sizeof(Vertex2D), size_t(GET_MEM_OFFSET(Vertex2D, texCoord)), false);
		attribs.vertex2dAttribs[2] = VertexAttribute::make(2, 4, DataType::FLOAT, sizeof(Vertex2D), size_t(GET_MEM_OFFSET(Vertex2D, color)), false);

		for (int i = 0; i < 4 * NUM_MAT4; i++)
			attribs.instancedMatAttribs[i] = VertexAttribute::make(i + 3, 4, DataType::FLOAT, sizeof(Mat4) * NUM_MAT4, size_t(i * sizeof(Vec4f)), true);
	}

	{
		m_shader = Shader::createSrc(shader::vert, shader::frag);
		m_instancedShader = Shader::createSrc(shader::instancedVert, shader::frag);
		m_defaultTexture = Texture::createBlank();
	}

	{

		Quad q = Quad::make();

		m_quadVAO = VertexArray::make();
		m_quadVAO.bind();

		m_quadBuffer = VertexBuffer::make(q.verts, Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW);

		setVertexLayout(m_quadBuffer, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

		unbindVAO();
	}

	{
		m_drawVAO = VertexArray::make();
		m_drawVAO.bind();

		m_instancedMatBuffer = VertexBuffer::createZeroed(sizeof(Mat4) * NUM_MAT4, 20, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);

		unbindVAO();
	}

}

void pure::Renderer::make(const Rectf & viewport)
{
	make();
	setViewport(viewport);
}

const Rectf & pure::Renderer::viewport() const { return m_viewport; }
const Mat4 &Renderer::projection() const { return m_projection; }

const Mat4 Renderer::MVMatrix() const { return m_projection * cam.view(); }

float Renderer::clipNear() { return CLIP_NEAR; }
float Renderer::clipFar() { return CLIP_FAR; }

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
		
        shader.setUniformIndx(Shader::MODEL_MAT_LOC, *transform);
        shader.setUniformIndx(Shader::MVP_MAT_LOC, m_projection * cam.view() * (*transform));
    }
    else
    {
        const Mat4 mat = Mat4::make();
        shader.setUniformIndx(Shader::MODEL_MAT_LOC, mat);
        shader.setUniformIndx(Shader::MVP_MAT_LOC, mat);
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

void pure::Renderer::drawMesh(const Mesh & m_mesh, const Mat4 & transform)
{
    m_drawVAO.bind();

    const Shader& shader = m_mesh.shader.id() == 0 ? m_shader : m_mesh.shader;
    shader.bind();

    if (m_mesh.texture) m_mesh.texture->bind();
    else m_defaultTexture.bind();

    m_mesh.vbo.bind();

    setVertexLayout(m_mesh.vbo, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    shader.setUniformIndx(Shader::MVP_MAT_LOC, m_projection * cam.view() * transform);
    shader.setUniformIndx(Shader::MODEL_MAT_LOC, transform);

	if (m_mesh.ebo.id_ != 0)
	{
		m_mesh.ebo.bind();
		drawElements(m_mesh.primtype, m_mesh.ebo.count);
		unbindEBO();
	}
	else
	{
		drawArrays(m_mesh.primtype, 0, uint32_t(m_mesh.vbo.vertCount));
	}

    unbindVBO();
}

void pure::Renderer::drawMeshStatic(const Mesh & m_mesh)
{
	drawBuffer(0, m_mesh.vbo.vertCount, m_mesh.vbo, m_mesh.texture, m_mesh.shader, 
		(m_mesh.ebo.id_ ? &m_mesh.ebo : nullptr), m_mesh.primtype);
}

void pure::Renderer::drawMeshInstanced(const Mesh & m_mesh, const Mat4 * transforms, uint32_t numDraws)
{
    m_drawVAO.bind();
    if (m_mesh.shader.id() == 0) m_instancedShader.bind();
    else m_mesh.shader.bind();

    if (m_mesh.texture) m_mesh.texture->bind();
    else m_defaultTexture.bind();

    setVertexLayout(m_mesh.vbo, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

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

    glDrawArraysInstanced(static_cast<GLenum>(m_mesh.primtype), 0, uint32_t(m_mesh.vbo.vertCount), numDraws);
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
    shader.setUniformIndx(Shader::MODEL_MAT_LOC, Mat4::make());
    shader.setUniformIndx(Shader::MVP_MAT_LOC, Mat4::make());

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

