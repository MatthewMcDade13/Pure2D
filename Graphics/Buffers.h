#ifndef PURE2D_GRAPHICS_BUFFERS_H
#define PURE2D_GRAPHICS_BUFFERS_H

#include <cinttypes>
#include <cstdio>
#include "Define.h"
#include "Math/Vec2.h"
#include "Math/Vec3.h"
#include "Math/Vec4.h"
#include "Math/Mat4.h"

namespace pure
{
	struct Vertex;
	struct Vertex2D;

	enum class DrawUsage
	{
		STATIC_DRAW = 0x88E4,
		DYNAMIC_DRAW = 0x88E8,
		STREAM_DRAW = 0x88E0
	};

	enum class BufferType
	{
		BYTE = 0x1400,
		UBYTE = 0x1401,
		SHORT = 0x1402,
		USHORT = 0x1403,
		INT = 0x1404,
		UINT = 0x1405,
		FLOAT = 0x1406
	};

	enum class DrawPrimitive
	{
		POINTS = 0x0000,
		LINES = 0x0001,
		LINE_LOOP = 0x0002,
		LINE_STRIP = 0x0003,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006,
		QUADS = 0x0007,
		QUAD_STRIP = 0x0008,
		POLYGON = 0x0009
	};

	struct PURE2D_API VertexAttribute
    {
        uint8_t bufferIndex;
        uint8_t elemCount;
        bool isInstanced;
        BufferType type : 16;
        size_t offset;
        size_t stride;

        static inline VertexAttribute create(uint8_t bufferIndex, uint8_t elemCount,
                BufferType type, size_t stride, size_t offset, bool isInstanced)
        {
            VertexAttribute va = {};
            va.bufferIndex = bufferIndex;
            va.elemCount = elemCount;
            va.type = type;
            va.offset = offset;
            va.stride = stride;
            va.isInstanced = isInstanced;
            return va;
        }
    };


	struct ElementBuffer
	{
		uint32_t id_;
		size_t count;
	};

	struct PURE2D_API VertexBuffer
	{
		uint32_t id_;
//		uint32_t vertexSize;
		BufferType type;
		size_t vertCount;
		size_t size;
//		size_t capacity;

		static VertexBuffer createZeroed(uint32_t typeSize, uint32_t count, DrawUsage usage, BufferType type);

		template<typename T>
		static VertexBuffer create(const T* verts, size_t count, DrawUsage usage, BufferType type = BufferType::FLOAT);

		template<typename T>
		void alloc(const T* verts, size_t count, DrawUsage usage, BufferType type = BufferType::FLOAT);

		template<typename T>
		void writeBuffer(const T* verts, size_t count, intptr_t bufferOffset);

		void bind() const;
		void free();

	};

	struct PURE2D_API VertexArray
	{
		uint32_t id_;

		static VertexArray create();

		void bind() const;
		void free();
		void setLayout(const VertexBuffer& buffer, uint32_t index, int elemCount, bool normalized, size_t stride, void* ptr);
	};

	PURE2D_API ElementBuffer createEBO(const uint32_t* indicies, size_t count, DrawUsage usage = DrawUsage::STATIC_DRAW);
	PURE2D_API void deleteEBO(ElementBuffer& buffer);

	PURE2D_API void unbindEBO();
	PURE2D_API void unbindVAO();
	PURE2D_API void unbindVBO();

	// TODO: Move this to renderer. We can store start, vertCount, and DrawPrimitive on VertexArray most likely...
	PURE2D_API void drawArrays(DrawPrimitive prim, uint32_t start, uint32_t vertCount);
	PURE2D_API void drawElements(DrawPrimitive prim, uint32_t count);

	PURE2D_API void setVertexLayout(const VertexBuffer& buffer, VertexAttribute* attribs, size_t numAttribs);
}

#endif // PURE2D_GRAPHICS_BUFFERS_H