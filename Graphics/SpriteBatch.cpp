//
// Created by matt on 10/1/18.
//

#include <cassert>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

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
        Shader batchShader;
        VertexBuffer vbo = {};
        int uniformLocations[2];
        ElementBuffer ebo;
    };

    enum { PROJ_MAT = 0, VIEW_MAT };

}

using namespace pure;

static constexpr const char* batchVertShader = "\n"
        "uniform mat4 u_viewMatrix;\n"
        "uniform mat4 u_projMatrix;\n"
        "vec4 position(mat4 mvpMat, vec3 pos)\n"
        "{\n"
        "   return u_projMatrix * (u_viewMatrix * vec4(pos, 1.0));\n"
        "}\n";

static constexpr const char* batchFragShader = "\n"
        "vec4 effect(vec4 color, sampler2D tex, vec2 texCoord, vec3 fragPos)\n"
        "{\n"
        "   return texture(tex, texCoord) * color;\n"
        "}\n";

static ElementBuffer createElementBuffer(size_t count);

static const size_t vertShaderlen = Shader::getVertShaderSize(strlen(batchVertShader));
static const size_t fragShaderlen = Shader::getFragShaderSize(strlen(batchFragShader));

pure::SpriteBatch::SpriteBatch(const pure::Texture &texture, size_t maxNumSprites):
    texture(&texture), m_impl(new SpriteBatch_Impl())
{
    std::string vert{};
    std::string frag{};

    vert.resize(vertShaderlen);
    frag.resize(fragShaderlen);

    Shader::createVertShader(&vert[0], batchVertShader, false);
    Shader::createFragShader(&frag[0], batchFragShader);

    m_impl->batchShader = Shader::createSrc(vert.c_str(), frag.c_str());
    m_impl->uniformLocations[PROJ_MAT] = m_impl->batchShader.getLocation("u_projMatrix");
    m_impl->uniformLocations[VIEW_MAT] = m_impl->batchShader.getLocation("u_viewMatrix");

    m_impl->ebo = createElementBuffer(maxNumSprites * 6);

    reset(maxNumSprites);
}

pure::SpriteBatch::~SpriteBatch()
{
    m_impl->vbo.free();
    m_impl->batchShader.free();
    deleteEBO(m_impl->ebo);
    delete m_impl;
    m_impl = nullptr;
}

void SpriteBatch::submit(const Quad& quad, const Mat4& transform)
{
    Quad q = quad;
    q.translate(transform);
    m_impl->quads.push_back(q);
}

void pure::SpriteBatch::flush()
{
    m_impl->quads.clear();
    m_impl->vbo.vertCount = 0;
}

void pure::SpriteBatch::reset(size_t maxNumSprites)
{
    if (m_impl->vbo.id_ != 0) m_impl->vbo.free();
    if (m_impl->ebo.id_ != 0) deleteEBO(m_impl->ebo);

    m_impl->vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), maxNumSprites * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
    m_impl->ebo = createElementBuffer(maxNumSprites * 6);

    flush();
    m_impl->quads.reserve(maxNumSprites);
}

void SpriteBatch::draw(Renderer& renderer)
{
    const auto* verts = reinterpret_cast<Vertex2D*>(&m_impl->quads[0]);
    const size_t numVerts = m_impl->quads.size() * Quad::VERT_COUNT;

    m_impl->vbo.writeBuffer(verts, numVerts, 0);
    m_impl->vbo.vertCount = numVerts;

    m_impl->batchShader.setUniform(m_impl->uniformLocations[PROJ_MAT], renderer.projection());
    m_impl->batchShader.setUniform(m_impl->uniformLocations[VIEW_MAT], renderer.cam.view());

    renderer.drawBuffer(0, m_impl->vbo.vertCount, m_impl->vbo, texture, m_impl->batchShader, &m_impl->ebo, DrawPrimitive::TRIANGLES);

}

ElementBuffer createElementBuffer(size_t count)
{
    std::vector<uint32_t> indices(count);

    const size_t itrRange = indices.size() / 6;

    for (size_t i = 0; i < itrRange; i++)
    {
        size_t quadIndex = i * 6;
        auto vertexIndex = static_cast<uint32_t>(i * 4);

        indices[quadIndex + 0] = vertexIndex + 0;
        indices[quadIndex + 1] = vertexIndex + 1;
        indices[quadIndex + 2] = vertexIndex + 2;

        indices[quadIndex + 3] = vertexIndex + 2;
        indices[quadIndex + 4] = vertexIndex + 3;
        indices[quadIndex + 5] = vertexIndex + 1;
    }

    return createEBO(&indices[0], indices.size());
}