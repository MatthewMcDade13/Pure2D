//
// Created by matt on 9/3/18.
//
#include "Renderer.h"
#include "Math/MatrixTransform.h"
#include "Math/Convert.h"
#include "Private/Convert.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderable.h"
#include <Pure2D/Graphics/RenderTexture.h>
#include "Graphics/Mesh.h"
#include "Window/Window.h"
#include "Private/Shaders.h"
#include "Private/GlContext.h"
#include "Private/Util.h"
#include <string>
#include <vector>
#include "External/glad.h"
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

void pure::Renderer::create(const Window& win)
{

	glViewport(0, 0, win.width(), win.height());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_targetWindow = &win;

	setViewport({ 0.f, 0.f, 
		static_cast<float>(win.width()), 
		static_cast<float>(win.height()) 
	});

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
		m_shader = Shader::fromSrc(shader::vert, shader::frag);
		m_instancedShader = Shader::fromSrc(shader::instancedVert, shader::frag);
		m_defaultTexture = Texture::createBlank();
	}

	{
		Quad q = Quad::make();

		m_quadBuffer = VertexBuffer::make(q.verts, Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW);
	}

	{
		m_drawVAO = VertexArray::make();
		m_drawVAO.bind();

		m_instancedMatBuffer = VertexBuffer::createZeroed(sizeof(Mat4) * NUM_MAT4, 20, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);
	}

}

void pure::Renderer::beginDrawTexture(const RenderTexture & rt)
{
	FrameBuffer::bind(rt.frameBuffer);
	m_userDrawTarget = &rt;
}

void pure::Renderer::endDrawTexture()
{
	FrameBuffer::unbind();
	m_userDrawTarget = nullptr;
}

const RenderTexture * pure::Renderer::drawTarget() const { return m_userDrawTarget; }

const Rectf & pure::Renderer::viewport() const { return m_viewport; }
const Mat4 &Renderer::projection() const { return m_projection; }

Mat4 Renderer::MVMatrix() const { return m_projection * cam.view(); }

float Renderer::clipNear() { return CLIP_NEAR; }
float Renderer::clipFar() { return CLIP_FAR; }

void pure::Renderer::clear(const Vec4f & color) const
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pure::Renderer::present() const
{
	m_targetWindow->swapBuffers();
}

void Renderer::drawQuad(const Quad &quad, const Mat4 *transform, const Texture *texture)
{
    drawQuad(quad, transform, m_shader, texture);
}

void Renderer::drawQuad(const Quad &quad, const Mat4 *transform, const Shader& shader, const Texture *texture)
{
    shader.bind();
    if (texture) texture->bind();
    else m_defaultTexture.bind();

	setVertexLayout(m_quadBuffer, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    if (transform)
    {
		
        shader.setUniformIndx(Shader::MODEL_MAT_LOC, *transform);
        shader.setUniformIndx(Shader::MVP_MAT_LOC, m_projection * cam.view() * (*transform));
    }
    else
    {
        const Mat4 mat = Mat4::identity();
        shader.setUniformIndx(Shader::MODEL_MAT_LOC, mat);
        shader.setUniformIndx(Shader::MVP_MAT_LOC, mat);
    }

    m_quadBuffer.writeBuffer(quad.verts, Quad::VERT_COUNT, 0);


    gl::drawArrays(DrawPrimitive::TRIANGLE_STRIP, 0, m_quadBuffer.vertCount);
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
    const Shader& shader = mesh.shader.id() == 0 ? m_shader : mesh.shader;
    shader.bind();

    if (mesh.texture) mesh.texture->bind();
    else m_defaultTexture.bind();

    //mesh.vbo.bind();

    setVertexLayout(mesh.vbo, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    shader.setUniformIndx(Shader::MVP_MAT_LOC, m_projection * cam.view() * transform);
    shader.setUniformIndx(Shader::MODEL_MAT_LOC, transform);

	if (mesh.ebo.id_ != 0)
	{
		mesh.ebo.bind();
		gl::drawElements(mesh.primtype, mesh.ebo.count);
		gl::unbindElementBuffer();
	}
	else
	{
		gl::drawArrays(mesh.primtype, 0, uint32_t(mesh.vbo.vertCount));
	}

	gl::unbindArrayBuffer();
}

void pure::Renderer::drawMeshStatic(const Mesh & m_mesh)
{
	drawBuffer(0, m_mesh.vbo.vertCount, m_mesh.vbo, m_mesh.texture, m_mesh.shader, 
		(m_mesh.ebo.id_ ? &m_mesh.ebo : nullptr), m_mesh.primtype);
}

void pure::Renderer::drawMeshInstanced(const Mesh & mesh, const Mat4 * transforms, uint32_t numDraws)
{
    if (mesh.shader.id() == 0) m_instancedShader.bind();
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

    glDrawArraysInstanced(toGlPrim(mesh.primtype), 0, uint32_t(mesh.vbo.vertCount), numDraws);
	gl::unbindArrayBuffer();
}

void Renderer::drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture, const ElementBuffer* ebo, DrawPrimitive primtype)
{
    drawBuffer(start, count, buffer, texture, m_shader, ebo, primtype);
}

void Renderer::drawBuffer(uint32_t start, uint32_t count, VertexBuffer buffer, const Texture *texture, const Shader& shader, const ElementBuffer* ebo, DrawPrimitive primtype)
{
    shader.bind();
    if (texture) texture->bind();
    else m_defaultTexture.bind();

	//buffer.bind();

    // TODO: Maybe we can use a different simple shader to avoid sending useless data to gpu?
    shader.setUniformIndx(Shader::MODEL_MAT_LOC, Mat4::identity());
    shader.setUniformIndx(Shader::MVP_MAT_LOC, Mat4::identity());

    setVertexLayout(buffer, attribs.vertex2dAttribs, ARRAY_COUNT(attribs.vertex2dAttribs));

    if (ebo)
    {
		ebo->bind();
        gl::drawElements(primtype, ebo->count);
		gl::unbindElementBuffer();
    }
    else
    {
        gl::drawArrays(primtype, start, count);
    }

	gl::unbindArrayBuffer();
}

void Renderer::draw(Renderable &renderable)
{
    renderable.draw(*this);
}

void Renderer::destroy()
{
    m_quadBuffer.free();
    m_instancedMatBuffer.free();

    m_instancedShader.free();
    m_shader.free();
    m_defaultTexture.free();

    m_drawVAO.free();
}

void pure::Renderer::activate()
{
	m_drawVAO.bind();
}

