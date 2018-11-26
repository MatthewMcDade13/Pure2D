#ifndef PURE2D_GRAPHICS_RENDERTEXTURE_H
#define PURE2D_GRAPHICS_RENDERTEXTURE_H

#include <cinttypes>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Buffers.h>

namespace pure
{
	struct RenderTexture
	{
		FrameBuffer buffer;
		Texture texture;

		static RenderTexture make(size_t w, size_t h);
	};
}

#endif // PURE2D_GRAPHICS_RENDERTEXTURE_H