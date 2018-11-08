#ifndef PURE2D_GRAPHICS_FONT_H
#define PURE2D_GRAPHICS_FONT_H

#include <Pure2D/Define.h>
#include <Pure2D/Graphics/Renderable.h>
#include <Pure2D/System/NonCopyable.h>
#include <Pure2D/Math/Vec2.h>
#include <Pure2D/Math/Vec3.h>
#include <Pure2D/Math/Rect.h>

namespace pure
{
	struct Transform;
	template<typename T> struct Vec4;

	struct Glyph
	{
		float xOffsetCoord;
		Vec2f advance;
		Rectf bitmap;
	};

	struct PURE2D_API Font : private NonCopyable
	{
		struct PURE2D_API Text : public Renderable
		{
			friend Font;

			Text();
			~Text();
			Text(Text&& other) noexcept;

			void setSize(Vec2f size);
			void setPosition(Vec3f pos);
			void setRotation(float rot);
			void setColor(const Vec4<float>& color);

			const Transform& transform() const;

			void draw(Renderer& renderer) final;

		private:
			// need this to keep text non-copyable but also allow Font to create it. 
			//FUCKING AIDS
			Text(const Text & other);
			// this creates incredibly dumb semantics for text. rethink this model of creating
			// text through font and/or having text be a child struct of font.
			Text& operator=(const Text& other) = delete;

			void updateVerts();

			struct Text_Impl* m_impl;
		};

		Font();
		~Font();

		void free();

		bool loadFromFile(const char* filename);
		void loadSize(int size);

		Text makeText(const char* text, int size);

	private:
		struct Font_Impl* m_impl;
	};
}


#endif // PURE2D_GRAPHICS_FONT_H

