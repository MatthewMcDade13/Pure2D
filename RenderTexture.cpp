#include "RenderTexture.h"
#include "External/glad.h"

using namespace pure;

RenderTexture pure::RenderTexture::make(size_t w, size_t h)
{
	RenderTexture rt = {};
	rt.buffer = FrameBuffer::make();
	rt.texture = Texture::make(w, h, Texture::Format::RGB, Texture::Format::RGB, nullptr);
	rt.buffer.attachTexture(rt.texture);

}
