//
// Created by matt on 10/1/18.
//

#include <cassert>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include <Pure2D/Graphics/Mesh.h>
#include "SpriteBatch.h"
#include "Graphics/Texture.h"
#include "Graphics/Quad.h"
#include "Graphics/Buffers.h"
#include "Graphics/Renderer.h"
#include "Quad.h"
#include "External/glad.h"

namespace pure
{

    struct SpriteBatch_Impl
    {
        std::vector<Quad> quads;
        int uniformLocations[2];
		Mesh sprites;
    };

    enum { PROJ_MAT = 0, VIEW_MAT };

}

using namespace pure;

static constexpr const char* batchVertShader = "\n"
        "uniform mat4 u_viewMatrix;\n"
        "uniform mat4 u_projMatrix;\n"
        "vec4 position(mat4 mvpMat, vec3 pos)\n"
        "{\n"
        "   FragPos = pos;"
        "   return u_projMatrix * (u_viewMatrix * vec4(pos, 1.0));\n"
        "}\n";

static constexpr const char* batchFragShader = "\n"
        "vec4 effect(vec4 color, sampler2D tex, vec2 texCoord, vec3 fragPos)\n"
        "{\n"
        "   return texture(tex, texCoord) * color;\n"
        "}\n";

static const size_t vertShaderlen = Shader::getVertShaderSize(strlen(batchVertShader));
static const size_t fragShaderlen = Shader::getFragShaderSize(strlen(batchFragShader));

pure::SpriteBatch::SpriteBatch(const pure::Texture &texture, size_t maxNumSprites):
    texture(&texture), m_impl(new SpriteBatch_Impl())
{
	reset(maxNumSprites);

    std::string vert{};
    std::string frag{};

    vert.resize(vertShaderlen);
    frag.resize(fragShaderlen);

    Shader::createVertShader(&vert[0], batchVertShader, false);
    Shader::createFragShader(&frag[0], batchFragShader);

    m_impl->sprites.shader = Shader::createSrc(vert.c_str(), frag.c_str());
    m_impl->uniformLocations[PROJ_MAT] = m_impl->sprites.shader.getLocation("u_projMatrix");
    m_impl->uniformLocations[VIEW_MAT] = m_impl->sprites.shader.getLocation("u_viewMatrix");
}

pure::SpriteBatch::~SpriteBatch()
{
    m_impl->sprites.vbo.free();
    m_impl->sprites.shader.free();
	m_impl->sprites.ebo.free();
    delete m_impl;
    m_impl = nullptr;
}

size_t SpriteBatch::submit(const Quad& quad, const Mat4& transform)
{
    Quad q = quad;
    q.translate(transform);
    m_impl->quads.push_back(q);
	return m_impl->quads.size() - 1;
}

Quad & pure::SpriteBatch::get(size_t index)
{
	assert(index >= 0 && index < m_impl->quads.size());
	return m_impl->quads[index];
}

void pure::SpriteBatch::flush()
{
    m_impl->quads.clear();
    m_impl->sprites.vbo.vertCount = 0;
}

void pure::SpriteBatch::reset(size_t maxNumSprites)
{
	Mesh& sprites = m_impl->sprites;
	sprites.vbo.free();
	sprites.ebo.free();

	sprites.primtype = DrawPrimitive::TRIANGLES;
	sprites.texture = this->texture;
	sprites.vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), maxNumSprites * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);
	sprites.ebo = ElementBuffer::quad(maxNumSprites * 6);

	flush();
	m_impl->quads.reserve(maxNumSprites);
}

void SpriteBatch::draw(Renderer& renderer)
{
    const auto* verts = reinterpret_cast<Vertex2D*>(&m_impl->quads[0]);
    const size_t numVerts = m_impl->quads.size() * Quad::VERT_COUNT;

    m_impl->sprites.vbo.writeBuffer(verts, numVerts, 0);
    m_impl->sprites.vbo.vertCount = numVerts;

    m_impl->sprites.shader.setUniform(m_impl->uniformLocations[PROJ_MAT], renderer.projection());
    m_impl->sprites.shader.setUniform(m_impl->uniformLocations[VIEW_MAT], renderer.cam.view());

	renderer.drawMeshStatic(m_impl->sprites);

}

void SpriteBatch::setFragShader(const char *shaderSrc)
{
    m_impl->sprites.shader.free();

    std::string vert{};
    vert.resize(vertShaderlen);
    Shader::createVertShader(&vert[0], batchVertShader, false);

    m_impl->sprites.shader = Shader::createSrc(vert.c_str(), shaderSrc);
}

const Shader &SpriteBatch::shader() const
{
    return m_impl->sprites.shader;
}

