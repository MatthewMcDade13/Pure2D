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
	struct Texture;

	enum class DrawUsage
	{
		STATIC_DRAW,
		DYNAMIC_DRAW,
		STREAM_DRAW
	};

	enum class DrawPrimitive
	{
		POINTS,
		LINES,
		LINE_LOOP,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_STRIP,
		TRIANGLE_FAN,
		QUADS,
		QUAD_STRIP,
		POLYGON
	};

	enum class BufferAccess
	{
		READ_ONLY,
		WRITE_ONLY,
		READ_WRITE
	};

	struct PURE2D_API RenderBuffer
	{
		uint32_t id;
		static RenderBuffer make(size_t w, size_t h);

		void bind() const;
		void free();
	};

	struct PURE2D_API FrameBuffer
	{
		uint32_t id;

		static FrameBuffer make();

		// Since binding to framebuffer 0 is considered the window,
		// we will make bind and unbind static methods instead of instance methods
		static void bind(FrameBuffer fb);
		static void unbind();

		void attachTexture(const Texture& tex, int attachmentNum = 0) const;
		void attachRenderBuffer(RenderBuffer rb);

		void free();
	};


	struct PURE2D_API VertexAttribute
    {
        uint8_t bufferIndex;
        uint8_t elemCount;
        bool isInstanced;
        DataType type;
        size_t offset;
        size_t stride;

        static inline VertexAttribute make(uint8_t bufferIndex, uint8_t elemCount,
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
		static ElementBuffer make(const uint32_t* indicies, size_t count, DrawUsage usage = DrawUsage::STATIC_DRAW);
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
		static VertexBuffer make(const T* verts, size_t count, DrawUsage usage, DataType type = DataType::FLOAT);

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

		static VertexArray make();

		void bind() const;
		void free();
		void setLayout(const VertexBuffer& buffer, uint32_t index, int elemCount, bool normalized, size_t stride, void* ptr);
	};

	PURE2D_API void setVertexLayout(const VertexBuffer& buffer, VertexAttribute* attribs, size_t numAttribs);
}

#endif // PURE2D_GRAPHICS_BUFFERS_H