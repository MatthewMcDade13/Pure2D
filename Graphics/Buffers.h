#ifndef PURE2D_GRAPHICS_BUFFERS_H
#define PURE2D_GRAPHICS_BUFFERS_H

#include <cinttypes>
#include <cstdio>
#include <Pure2D/System/DataType.h>
#include <Pure2D/Define.h>
#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Vec3.h>
#include <Pure2D/Math/Vec4.h>
#include <Pure2D/Math/Mat4.h>

namespace pure
{

	enum class DrawUsage
	{
		STATIC_DRAW = 0x88E4,
		DYNAMIC_DRAW = 0x88E8,
		STREAM_DRAW = 0x88E0
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

	enum class BufferAccess
	{
		READ_ONLY  = 0x88B8,
		WRITE_ONLY = 0x88B9,
		READ_WRITE = 0x88BA
	};

	struct PURE2D_API VertexAttribute
    {
        uint8_t bufferIndex;
        uint8_t elemCount;
        bool isInstanced;
        DataType type : 16;
        size_t offset;
        size_t stride;

        static inline VertexAttribute create(uint8_t bufferIndex, uint8_t elemCount,
                DataType type, size_t stride, size_t offset, bool isInstanced)
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

	// TODO: ElementBuffer needs some love to conform to rest of API.
	struct PURE2D_API ElementBuffer
	{
		static ElementBuffer create(const uint32_t* indicies, size_t count, DrawUsage usage = DrawUsage::STATIC_DRAW);
		static ElementBuffer quad(size_t count);

		void bind() const;

		void free();

		uint32_t id_;
		size_t count;
	};

	struct PURE2D_API VertexBuffer
	{
		uint32_t id_;
//		uint32_t vertexSize;
		DataType type;
		size_t vertCount;
		size_t size;
//		size_t capacity;

		static VertexBuffer createZeroed(size_t typeSize, size_t count, DrawUsage usage, DataType type);

		template<typename T>
		static VertexBuffer create(const T* verts, size_t count, DrawUsage usage, DataType type = DataType::FLOAT);

		template<typename T>
		void alloc(const T* verts, size_t count, DrawUsage usage, DataType type = DataType::FLOAT);

		template<typename T>
		void writeBuffer(const T* verts, size_t count, intptr_t bufferOffset);

		void copyData(VertexBuffer other, intptr_t readOffset, intptr_t writeOffset, size_t size);

		void* map(BufferAccess access = BufferAccess::READ_WRITE);
		void unmap();

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

	PURE2D_API void unbindEBO();
	PURE2D_API void unbindVAO();
	PURE2D_API void unbindVBO();

	PURE2D_API void drawArrays(DrawPrimitive prim, uint32_t start, uint32_t vertCount);

	// TODO: Move this to renderer.
	PURE2D_API void drawElements(DrawPrimitive prim, uint32_t count);

	PURE2D_API void setVertexLayout(const VertexBuffer& buffer, VertexAttribute* attribs, size_t numAttribs);
}

#endif // PURE2D_GRAPHICS_BUFFERS_H