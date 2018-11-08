//
// Created by matt on 10/1/18.
//

#ifndef PURE2D_SPRITEBATCH_H
#define PURE2D_SPRITEBATCH_H

#include <cstddef>
#include <vector>
#include <Pure2D/Define.h>
#include <Pure2D/Graphics/Renderable.h>
#include <Pure2D/System/NonCopyable.h>
#include <Pure2D/Graphics/Mesh.h>

namespace pure
{
    struct Renderer;
    struct Texture;
    struct Shader;
    struct Quad;
    struct Mat4;

    // TODO: Implement free() function like rest of api?
    struct PURE2D_API SpriteBatch : public Renderable
    {
        const Texture* texture;

        SpriteBatch(const Texture& texture, size_t maxNumSprites);
        ~SpriteBatch();

        const Shader& shader() const;
        void setFragShader(const char* shaderSrc);

        void reset(size_t maxNumSprites);
		// submits quad to SpriteBatch. Returns id index to quad in batch
        size_t submit(const Quad& quad, const Mat4& transform);
		Quad& get(size_t index);

		void flush();

        void draw(Renderer& renderer) final;


    private:
		std::vector<Quad> m_quads;
		int m_uniformLocations[2];
		Mesh m_sprites;
    };
}


#endif //PURE2D_SPRITEBATCH_H
