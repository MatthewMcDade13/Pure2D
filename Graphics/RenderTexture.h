#ifndef PURE2D_GRAPHICS_RENDERTEXTURE_H
#define PURE2D_GRAPHICS_RENDERTEXTURE_H

#include <cinttypes>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Buffers.h>
#include <Pure2D/Math/Vec4.h>

namespace pure
{
	struct RenderTexture
	{
		FrameBuffer frameBuffer;
		RenderBuffer renderBuffer;
		Texture texture;

		static RenderTexture make(size_t w, size_t h);

		void clear(Vec4f color = { 0.f, 0.f, 0.f, 1.f });
		void free();
	};
}

#endif // PURE2D_GRAPHICS_RENDERTEXTURE_H