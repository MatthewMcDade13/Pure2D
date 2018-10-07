//
// Created by matt on 10/1/18.
//

#ifndef PURE2D_SPRITEBATCH_H
#define PURE2D_SPRITEBATCH_H

#include <cstddef>
#include "Graphics/Renderable.h"
#include "System/NonCopyable.h"

namespace pure
{
    struct Renderer;
    struct Texture;
    struct Shader;
    struct Quad;
    struct Mat4;

    // TODO: To make this Renderable or not to make this Renderable...
    struct SpriteBatch : public Renderable, private NonCopyable
    {
        const Texture* texture;

        SpriteBatch(const Texture& texture, size_t maxNumSprites);
        ~SpriteBatch();

        const Shader& shader() const;
        void setFragShader(const char* shaderSrc);

        void reset(size_t maxNumSprites);
        void submit(const Quad& quad, const Mat4& transform);
        void flush();

        void draw(Renderer& renderer) final;


    private:
        struct SpriteBatch_Impl* m_impl;
    };
}


#endif //PURE2D_SPRITEBATCH_H
