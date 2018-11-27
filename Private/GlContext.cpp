#include "GlContext.h"
#
#include "Private/Convert.h"
#include "External/glad.h"
#include <cassert>

static gl::ActiveStates stateCache = {};

void gl::clearActiveVAOState()
{
	stateCache.vaoStates[stateCache.activeVAO] = {};
}

const gl::ActiveStates & gl::getStates()
{
	return stateCache;
}

uint32_t gl::getBindState(BindState bs)
{
	assert(bs >= 0 && bs < BindState::NUM_STATES);
	return stateCache.bindStates[bs];
}

bool gl::isStateBound(BindState bs, uint32_t id)
{
	assert(bs >= 0 && bs < BindState::NUM_STATES);
	return stateCache.bindStates[bs] == id;
}

bool gl::isArrayBufferBound(uint32_t buffer)
{
	return stateCache.vaoStates[stateCache.activeVAO].vbo == buffer;
}

bool gl::isElementBufferBound(uint32_t buffer)
{
	return stateCache.vaoStates[stateCache.activeVAO].ebo == buffer;
}

void gl::bindElementBuffer(uint32_t ebo)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	stateCache.vaoStates[stateCache.activeVAO].ebo = ebo;
}

void gl::unbindElementBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	stateCache.vaoStates[stateCache.activeVAO].ebo = 0;
}

void gl::bindArrayBuffer(uint32_t vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	stateCache.vaoStates[stateCache.activeVAO].vbo = vbo;
}

void gl::unbindArrayBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	stateCache.vaoStates[stateCache.activeVAO].vbo = 0;
}

void gl::bindVertexArray(uint32_t vao)
{
	glBindVertexArray(vao);
	stateCache.activeVAO = vao;
}

void gl::unbindVertexArray()
{
	glBindVertexArray(0);
	stateCache.activeVAO = 0;
}

void gl::bindFrameBuffer(uint32_t fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	stateCache.bindStates[FRAME_BUFFER] = fbo;
}

void gl::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	stateCache.bindStates[FRAME_BUFFER] = 0;
}

void gl::bindRenderBuffer(uint32_t rbo)
{
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	stateCache.bindStates[RENDER_BUFFER] = rbo;
}

void gl::unbindRenderBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	stateCache.bindStates[RENDER_BUFFER] = 0;
}

void gl::bindShader(uint32_t shader)
{
	glUseProgram(shader);
	stateCache.bindStates[SHADER_PROGRAM] = shader;
}

void gl::unbindShader()
{
	glUseProgram(0);
	stateCache.bindStates[SHADER_PROGRAM] = 0;
}

void gl::bindTexture(uint32_t texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	stateCache.bindStates[TEXTURE] = texture;
}

void gl::unbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	stateCache.bindStates[TEXTURE] = 0;
}

void gl::bindCopyReadBuffer(uint32_t id)
{
	glBindBuffer(GL_COPY_READ_BUFFER, id);
	stateCache.bindStates[COPY_READ_BUFFER] = id;
}

void gl::unbindCopyReadBuffer()
{
	glBindBuffer(GL_COPY_READ_BUFFER, 0);
	stateCache.bindStates[COPY_READ_BUFFER] = 0;
}

void gl::bindCopyWriteBuffer(uint32_t id)
{
	glBindBuffer(GL_COPY_WRITE_BUFFER, id);
	stateCache.bindStates[COPY_WRITE_BUFFER] = id;
}

void gl::unbindCopyWriteBuffer()
{
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	stateCache.bindStates[COPY_WRITE_BUFFER] = 0;
}

void gl::drawArrays(pure::DrawPrimitive prim, uint32_t start, uint32_t vertCount)
{
	glDrawArrays(toGlPrim(prim), start, vertCount);
}

void gl::drawElements(pure::DrawPrimitive prim, uint32_t count)
{
	glDrawElements(toGlPrim(prim), count, GL_UNSIGNED_INT, 0);
}
