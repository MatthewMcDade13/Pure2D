#include "Buffers.h"
#include "External/glad.h"
#include "Vertex.h"
#include <vector>
#include <iostream>
#include "Private/Convert.h"
#include "Private/GlContext.h"
#include <Pure2D/Graphics/Texture.h>
#include <cassert>

using namespace pure;


template<typename T>
static void deleteBuffer(T& buffer)
{
	glDeleteBuffers(1, &buffer.id_);
	buffer.id_ = 0;
}

ElementBuffer ElementBuffer::quad(size_t count)
{
	std::vector<uint32_t> indices(count);

	const size_t itrRange = indices.size() / 6;

	for (size_t i = 0; i < itrRange; i++)
	{
		size_t quadIndex = i * 6;
		auto vertexIndex = static_cast<uint32_t>(i * 4);

		indices[quadIndex + 0] = vertexIndex + 0;
		indices[quadIndex + 1] = vertexIndex + 1;
		indices[quadIndex + 2] = vertexIndex + 2;

		indices[quadIndex + 3] = vertexIndex + 2;
		indices[quadIndex + 4] = vertexIndex + 3;
		indices[quadIndex + 5] = vertexIndex + 1;
	}

	return ElementBuffer::make(&indices[0], indices.size());
}

void ElementBuffer::bind() const
{
	if (gl::isElementBufferBound(id_)) return;

	gl::bindElementBuffer(id_);
}

ElementBuffer ElementBuffer::make(const uint32_t* indicies, size_t count, DrawUsage usage)
{
	uint32_t ebo;
	glGenBuffers(1, &ebo);

	gl::bindElementBuffer(ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, toGlUsage(usage));

	return { ebo, count };
}

void ElementBuffer::free()
{
	glDeleteBuffers(1, &id_);
	id_ = 0;
}

void pure::setVertexLayout(const VertexBuffer &buffer, VertexAttribute *attribs, size_t numAttribs)
{
    //buffer.bind();
	gl::bindArrayBuffer(buffer.id_);
    for (size_t i = 0; i < numAttribs; i++)
    {
        VertexAttribute& attrib = attribs[i];

        glVertexAttribPointer(attrib.bufferIndex, attrib.elemCount,
                toGlDataType(attrib.type), GL_FALSE,
                static_cast<GLsizei>(attrib.stride), (void*)attrib.offset);
        glEnableVertexAttribArray(attrib.bufferIndex);

        glVertexAttribDivisor(attrib.bufferIndex, attrib.isInstanced ? 1 : 0);
    }
}

VertexBuffer pure::VertexBuffer::createZeroed(size_t typeSize, size_t count, DrawUsage usage, DataType type)
{
	uint32_t id;
	glGenBuffers(1, &id);

	const size_t nBytes = typeSize * count;

	gl::bindArrayBuffer(id);
	glBufferData(GL_ARRAY_BUFFER, nBytes, nullptr, toGlUsage(usage));

	return { id, type, 0, nBytes };
}

void pure::VertexBuffer::bind() const
{
	// TODO: WHY THE FUCK IS THIS NOT WORKING?!?!?!?
	// It looks like it is preventing binding at some point and
	// the default quad vao and the user draw vao arent getting 
	// proper vbos bound or some shit? i am literally at a fucking loss and
	// on the fucking verge of suicide.

	if (gl::isArrayBufferBound(id_))
	{
		return;
	}
	gl::bindArrayBuffer(id_);
}

VertexArray pure::VertexArray::make()
{
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	return { vao };
}

void pure::VertexArray::bind() const
{
	if (gl::getStates().activeVAO == id_) return;

	gl::bindVertexArray(id_);
}

void pure::VertexBuffer::free()
{
	deleteBuffer(*this);
}

void pure::VertexArray::free()
{
	deleteBuffer(*this);
}

void pure::VertexArray::setLayout(const VertexBuffer & buffer, uint32_t index, int elemCount, bool normalized, size_t stride, void * ptr)
{
	buffer.bind();

	glVertexAttribPointer(index, elemCount, toGlDataType(buffer.type), normalized, stride, ptr);
	glEnableVertexAttribArray(index);
}

// TODO/NOTE: Maybe consider taking quadVerts as void* and having extra parameter as type size? Might be a little more flexible
// and avoid having to maintain all these specializations. OR We could wrap all OpenGL calls in our own funcs and just use those
// funcs directly in template

template<typename T>
void pure::VertexBuffer::alloc(const T* verts, size_t count, DrawUsage usage, DataType type)
{
	bind();
	size = count * sizeof(T);
	glBufferData(GL_ARRAY_BUFFER, size, verts, toGlUsage(usage));
}

template<typename T>
void pure::VertexBuffer::writeBuffer(const T * verts, size_t count, intptr_t bufferOffset)
{
	bind();

	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, sizeof(T) * count, verts);
}

template<typename T>
VertexBuffer pure::VertexBuffer::make(const T * verts, size_t count, DrawUsage usage, DataType type)
{
	uint32_t id;
	glGenBuffers(1, &id);

	const size_t nBytes = sizeof(T) * count;

	VertexBuffer vb = { id, type, count, nBytes };

	vb.alloc(verts, count, usage, type);

	return vb;
}

void *VertexBuffer::map(BufferAccess access)
{
	bind();
    return glMapBuffer(GL_ARRAY_BUFFER, toGlBuffAccess(access));
}

void VertexBuffer::unmap()
{
	bind();
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VertexBuffer::copyData(VertexBuffer other, intptr_t readOffset, intptr_t writeOffset, size_t size)
{
	gl::bindCopyReadBuffer(other.id_);
	gl::bindCopyWriteBuffer(id_);

    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
}


// TODO: float overload is broken and doesn't properly set offsets and count/capacity of buffers
template VertexBuffer VertexBuffer::make<Vertex2D>(const Vertex2D* verts, size_t count, DrawUsage usage, DataType type);
template VertexBuffer VertexBuffer::make<float>(const float* verts, size_t count, DrawUsage usage, DataType type);
template VertexBuffer VertexBuffer::make<Vec2f>(const Vec2f* verts, size_t count, DrawUsage usage, DataType type);
template VertexBuffer VertexBuffer::make<Vec3f>(const Vec3f* verts, size_t count, DrawUsage usage, DataType type);
template VertexBuffer VertexBuffer::make<Vec4f>(const Vec4f* verts, size_t count, DrawUsage usage, DataType type);
template VertexBuffer VertexBuffer::make<Mat4>(const Mat4* verts, size_t count, DrawUsage usage, DataType type);

template void VertexBuffer::alloc<Vertex2D>(const Vertex2D* verts, size_t count, DrawUsage usage, DataType type);
template void VertexBuffer::alloc<float>(const float* verts, size_t count, DrawUsage usage, DataType type);
template void VertexBuffer::alloc<Vec2f>(const Vec2f* verts, size_t count, DrawUsage usage, DataType type);
template void VertexBuffer::alloc<Vec3f>(const Vec3f* verts, size_t count, DrawUsage usage, DataType type);
template void VertexBuffer::alloc<Vec4f>(const Vec4f* verts, size_t count, DrawUsage usage, DataType type);
template void VertexBuffer::alloc<Mat4>(const Mat4* verts, size_t count, DrawUsage usage, DataType type);

template void VertexBuffer::writeBuffer<Vertex2D>(const Vertex2D* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<float>(const float* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vec2f>(const Vec2f* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vec3f>(const Vec3f* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vec4f>(const Vec4f* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Mat4>(const Mat4* verts, size_t count, intptr_t bufferOffset);

FrameBuffer pure::FrameBuffer::make()
{
	FrameBuffer buff = {};
	glGenFramebuffers(1, &buff.id);
	bind(buff);
	return buff;
}

void pure::FrameBuffer::bind(FrameBuffer fb)
{
	if (gl::isStateBound(gl::BindState::FRAME_BUFFER, fb.id)) return;
	gl::bindFrameBuffer(fb.id);
}

void pure::FrameBuffer::unbind()
{
	if (gl::isStateBound(gl::BindState::FRAME_BUFFER, 0)) return;
	gl::unbindFrameBuffer();
}

void pure::FrameBuffer::attachTexture(const Texture & tex, int attachmentNum) const
{
	bind(*this);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNum, GL_TEXTURE_2D, tex.id_, 0);
}

void pure::FrameBuffer::attachRenderBuffer(RenderBuffer rb)
{
	bind(*this);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb.id);
}

void pure::FrameBuffer::free()
{
	glDeleteFramebuffers(1, &id);
	id = 0;
}

RenderBuffer pure::RenderBuffer::make(size_t w, size_t h)
{
	RenderBuffer rb = {};

	glGenRenderbuffers(1, &rb.id);
	rb.bind();

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

	return rb;
}

void pure::RenderBuffer::bind() const
{
	if (gl::isStateBound(gl::BindState::RENDER_BUFFER, id)) return;
	gl::bindRenderBuffer(id);
}

void pure::RenderBuffer::free()
{
	glDeleteRenderbuffers(1, &id);
	id = 0;
}
