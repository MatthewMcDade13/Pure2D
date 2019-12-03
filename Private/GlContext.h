#pragma once

#include <Pure2D/Graphics/Buffers.h>
#include <unordered_map>

namespace gl
{

	enum BindState
	{
		FRAME_BUFFER,
		RENDER_BUFFER,
		SHADER_PROGRAM,
		TEXTURE,
		// Does VAO capture these 2 buffers?
		COPY_READ_BUFFER,
		COPY_WRITE_BUFFER,
		NUM_STATES
	};

	struct VAOState
	{
		uint32_t vbo;
		uint32_t ebo;
	};


	struct ActiveStates
	{
		uint32_t activeVAO;
		std::unordered_map<uint32_t, VAOState> vaoStates;
		uint32_t bindStates[NUM_STATES];
	};

	void clearActiveVAOState();

	const ActiveStates& getStates();

	uint32_t getBindState(BindState bs);

	bool isStateBound(BindState bs, uint32_t id);

	bool isArrayBufferBound(uint32_t buffer);
	bool isElementBufferBound(uint32_t buffer);

	void bindElementBuffer(uint32_t ebo);
	void unbindElementBuffer();

	// TODO/NOTE: Seems to be some wierdness happening when switching
	// between 2 vaos. something isnt getting bound at right time and skipping when it shouldnt.
	// Isolated down to skipping ArrayBuffer (VBOs)
	// Observed in Renderer when we had 2 internal VAOs
	void bindArrayBuffer(uint32_t vbo);
	void unbindArrayBuffer();

	void bindVertexArray(uint32_t vao);
	void unbindVertexArray();

	void bindFrameBuffer(uint32_t fbo);
	void unbindFrameBuffer();

	void bindRenderBuffer(uint32_t fbo);
	void unbindRenderBuffer();

	void bindShader(uint32_t shader);
	void unbindShader();

	void bindTexture(uint32_t texture);
	void unbindTexture();

	void bindCopyReadBuffer(uint32_t id);
	void unbindCopyReadBuffer();

	void bindCopyWriteBuffer(uint32_t id);
	void unbindCopyWriteBuffer();

	void drawArrays(pure::DrawPrimitive prim, uint32_t start, uint32_t vertCount);

	void drawElements(pure::DrawPrimitive prim, uint32_t count);

}
