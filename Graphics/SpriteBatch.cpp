//
// Created by matt on 10/1/18.
//

#include <cassert>
#include "SpriteBatch.h"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"

pure::SpriteBatch::SpriteBatch(pure::Texture &texture, size_t maxNumSprites):
    texture(texture), m_writeOffset(0),  m_vbo({ })
{
    reset(maxNumSprites);
}

pure::SpriteBatch::~SpriteBatch()
{
    m_vbo.free();
}

void pure::SpriteBatch::submit(pure::Mesh &mesh)
{
    const size_t dataSize = mesh.vbo.vertCount * sizeof(Vertex2D);

    assert(dataSize + m_writeOffset <= m_vbo.size);

    m_vbo.copy(mesh.vbo, 0, m_writeOffset, dataSize);
    m_writeOffset += dataSize;
    m_vbo.vertCount += Mesh::QUAD_VERT_COUNT;
}

void pure::SpriteBatch::flush()
{
    m_writeOffset = 0;
    m_vbo.vertCount = 0;
}

void pure::SpriteBatch::draw(pure::Renderer &renderer)
{
    renderer.drawTriangles(0, m_vbo.vertCount, m_vbo, &texture);
}

void pure::SpriteBatch::reset(size_t maxNumSprites)
{
    if (m_vbo.id_ != 0) m_vbo.free();
    m_vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), maxNumSprites * Mesh::QUAD_VERT_COUNT, DrawUsage::DYNAMIC_DRAW, BufferType::FLOAT);
}

