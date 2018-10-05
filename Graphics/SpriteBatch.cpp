//
// Created by matt on 10/1/18.
//

#include <cassert>
#include <vector>

#include "SpriteBatch.h"
#include "Graphics/Texture.h"
#include "Graphics/Quad.h"
#include "Graphics/Buffers.h"
#include "Graphics/Renderer.h"
#include "Quad.h"

namespace pure
{
    struct SpriteBatch_Impl
    {
        std::vector<Quad> quads;
        VertexBuffer vbo = {};
        Renderer* renderer;
        intptr_t writeOffset = 0;
    };
}


using namespace pure;

pure::SpriteBatch::SpriteBatch(Renderer& renderer, const pure::Texture &texture, size_t maxNumSprites):
    texture(&texture), m_impl(new SpriteBatch_Impl())
{
    m_impl->quads.reserve(maxNumSprites);
    m_impl-> renderer = &renderer;
    reset(maxNumSprites);
}

pure::SpriteBatch::~SpriteBatch()
{
    m_impl->vbo.free();
    delete m_impl;
    m_impl = nullptr;
}

void SpriteBatch::submit(const Quad& quad, const Mat4& transform)
{
    Quad q = quad;
    translateVerts(q.verts, Quad::VERT_COUNT, m_impl->renderer->MVMatrix() * transform);
    m_impl->quads.push_back(q);
}

void pure::SpriteBatch::flush()
{
    m_impl->quads.clear();
    m_impl->writeOffset = 0;
    m_impl->vbo.vertCount = 0;
}

void pure::SpriteBatch::reset(size_t maxNumSprites)
{
    if (m_impl->vbo.id_ != 0) m_impl->vbo.free();
    m_impl->vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), maxNumSprites * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
}

void SpriteBatch::draw() const
{
    auto* verts = reinterpret_cast<Vertex2D*>(&m_impl->quads[0]);
    const size_t numVerts = m_impl->quads.size() * Quad::VERT_COUNT;

    m_impl->vbo.writeBuffer(verts, numVerts, 0);
    m_impl->vbo.vertCount = numVerts;
    m_impl->renderer->drawBuffer(0, m_impl->vbo.vertCount, m_impl->vbo, texture);

}

