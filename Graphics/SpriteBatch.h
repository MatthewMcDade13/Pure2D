//
// Created by matt on 10/1/18.
//

#ifndef PURE2D_SPRITEBATCH_H
#define PURE2D_SPRITEBATCH_H

#include "Graphics/Buffers.h"
#include "Graphics/Renderable.h"
#include "System/NonCopyable.h"

namespace pure
{
    struct Renderer;
    struct Texture;
    struct Mesh;

    struct SpriteBatch : public Renderable, private NonCopyable
    {
        const Texture* texture;

        explicit SpriteBatch(const Texture& texture, size_t maxNumSprites);
        ~SpriteBatch();

        void reset(size_t maxNumSprites);
        void submit(Mesh& mesh);
        void flush();

        void draw(Renderer& renderer) final;

    private:
        VertexBuffer m_vbo;
        intptr_t m_writeOffset;
    };
}


#endif //PURE2D_SPRITEBATCH_H
