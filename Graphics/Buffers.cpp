#include "Buffers.h"
#include "glad.h"
#include "Vertex.h"
#include <vector>
#include <iostream>

using namespace pure;

static constexpr GLenum mapToGLUsage(DrawUsage usage)
{
	switch (usage)
	{
	case DrawUsage::STATIC_DRAW: return GL_STATIC_DRAW;
	case DrawUsage::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
	case DrawUsage::STREAM_DRAW: return GL_STREAM_DRAW;
	}

	return 0;
}


template<typename T>
static void deleteBuffer(T& buffer)
{
	glDeleteBuffers(1, &buffer.id_);
	buffer.id_ = 0;
}

ElementBuffer pure::createEBO(const uint32_t* indicies, size_t count, DrawUsage usage)
{
	uint32_t ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, static_cast<GLenum>(usage));

	return { ebo, count };
}

void pure::deleteEBO(ElementBuffer & buffer)
{
	glDeleteBuffers(1, &buffer.id_);
	buffer.id_ = 0;
}

void pure::unbindVAO()
{
	glBindVertexArray(0);
}

void pure::unbindEBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void pure::unbindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void pure::drawArrays(DrawPrimitive prim, uint32_t start, uint32_t vertCount)
{
	glDrawArrays(static_cast<GLenum>(prim), start, vertCount);
}

void pure::drawElements(DrawPrimitive prim, uint32_t count)
{
	glDrawElements(static_cast<GLenum>(prim), count, GL_UNSIGNED_INT, 0);
}

VertexBuffer pure::VertexBuffer::createZeroed(uint32_t typeSize, uint32_t count, DrawUsage usage, BufferType type)
{
	uint32_t id;
	glGenBuffers(1, &id);

	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, typeSize * count, nullptr, static_cast<GLenum>(usage));

	return { id, typeSize, type, count, count };
}

void pure::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id_);
}

VertexArray pure::VertexArray::create()
{
	uint32_t vao;
	glGenVertexArrays(1, &vao);
	return { vao, 0 };
}

void pure::VertexArray::bind() const
{
	glBindVertexArray(id_);
}

void pure::VertexBuffer::free()
{
	deleteBuffer(*this);
}

void pure::VertexArray::free()
{
	deleteBuffer(*this);
}

void pure::VertexArray::setLayout(const VertexBuffer & buffer, int elemCount, bool normalized, size_t stride, void * ptr)
{
	buffer.bind();

	glVertexAttribPointer(numLayout, elemCount, static_cast<GLenum>(buffer.type), normalized, stride, ptr);
	glEnableVertexAttribArray(numLayout);

	numLayout++;
}

// TODO/NOTE: Maybe consider taking verts as void* and having extra parameter as type size? Might be a little more flexible
// and avoid having to maintain all these specializations. OR We could wrap all OpenGL calls in our own funcs and just use those
// funcs directly in template

template<typename T>
void pure::VertexBuffer::alloc(const T* verts, size_t count, DrawUsage usage, BufferType type)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), verts, static_cast<GLenum>(usage));
}

template<typename T>
void pure::VertexBuffer::writeBuffer(const T * verts, size_t count, intptr_t bufferOffset)
{
	bind();
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, sizeof(T) * count, verts);
}

template<typename T>
VertexBuffer pure::VertexBuffer::create(const T * verts, size_t count, DrawUsage usage, BufferType type)
{
	uint32_t id;
	glGenBuffers(1, &id);

	VertexBuffer vb = { id, sizeof(T), type, count, count };

	vb.alloc(verts, count, usage, type);

	return vb;
}


// TODO: float overload is broken and doesn't properly set offsets and count/capacity of buffers
template VertexBuffer VertexBuffer::create<Vertex>(const Vertex* verts, size_t count, DrawUsage usage, BufferType type);
template VertexBuffer VertexBuffer::create<Vertex2D>(const Vertex2D* verts, size_t count, DrawUsage usage, BufferType type);
template VertexBuffer VertexBuffer::create<float>(const float* verts, size_t count, DrawUsage usage, BufferType type);
template VertexBuffer VertexBuffer::create<Vec2f>(const Vec2f* verts, size_t count, DrawUsage usage, BufferType type);
template VertexBuffer VertexBuffer::create<Vec3f>(const Vec3f* verts, size_t count, DrawUsage usage, BufferType type);
template VertexBuffer VertexBuffer::create<Vec4f>(const Vec4f* verts, size_t count, DrawUsage usage, BufferType type);
template VertexBuffer VertexBuffer::create<Mat4>(const Mat4* verts, size_t count, DrawUsage usage, BufferType type);

template void VertexBuffer::alloc<Vertex>(const Vertex* verts, size_t count, DrawUsage usage, BufferType type);
template void VertexBuffer::alloc<Vertex2D>(const Vertex2D* verts, size_t count, DrawUsage usage, BufferType type);
template void VertexBuffer::alloc<float>(const float* verts, size_t count, DrawUsage usage, BufferType type);
template void VertexBuffer::alloc<Vec2f>(const Vec2f* verts, size_t count, DrawUsage usage, BufferType type);
template void VertexBuffer::alloc<Vec3f>(const Vec3f* verts, size_t count, DrawUsage usage, BufferType type);
template void VertexBuffer::alloc<Vec4f>(const Vec4f* verts, size_t count, DrawUsage usage, BufferType type);
template void VertexBuffer::alloc<Mat4>(const Mat4* verts, size_t count, DrawUsage usage, BufferType type);

template void VertexBuffer::writeBuffer<Vertex>(const Vertex* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vertex2D>(const Vertex2D* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<float>(const float* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vec2f>(const Vec2f* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vec3f>(const Vec3f* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Vec4f>(const Vec4f* verts, size_t count, intptr_t bufferOffset);
template void VertexBuffer::writeBuffer<Mat4>(const Mat4* verts, size_t count, intptr_t bufferOffset);