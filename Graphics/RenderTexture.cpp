#include "RenderTexture.h"
#include <iostream>
#include "External/glad.h"

using namespace pure;

RenderTexture pure::RenderTexture::make(size_t w, size_t h)
{
	RenderTexture rt = {};
	rt.frameBuffer = FrameBuffer::make();
	rt.texture = Texture::make(w, h, Texture::Format::RGB, Texture::Format::RGB, nullptr);
	rt.renderBuffer = RenderBuffer::make(w, h);

	rt.frameBuffer.attachTexture(rt.texture);
	rt.frameBuffer.attachRenderBuffer(rt.renderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		rt.free();
		std::cout << "OpenGL :: Could not create RenderTexture. Framebuffer is not complete" << std::endl;
	}

	rt.frameBuffer.unbind();

	return rt;
}


void pure::RenderTexture::clear(Vec4f color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void pure::RenderTexture::free()
{
	frameBuffer.free();
	renderBuffer.free();
	texture.free();
}
