#ifndef PURE2D_GRAPHICS_FONT_H
#define PURE2D_GRAPHICS_FONT_H

#include <vector>
#include <string>
#include <unordered_map>
#include <Pure2D/Define.h>
#include <Pure2D/Graphics/Renderable.h>
#include <Pure2D/System/NonCopyable.h>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Transform.h>
#include <Pure2D/Graphics/Quad.h> 
#include <Pure2D/Graphics/Mesh.h>
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

	struct Text;

	struct PURE2D_API Font : private NonCopyable
	{
		friend Text;
		void free();

		bool loadFromFile(const char* filename);
		void loadSize(int size);

		Text makeText(const char* text, int size);

		Vec2i getKerning(char left, char right);

	private:
		static constexpr int MAX_NUM_CHARS = 128;
		struct FontMap
		{
			Glyph glyphs[MAX_NUM_CHARS] = {};
			float height = 0.f;
			float width = 0.f;
		};

		// fonts cached with thier heights as keys
		std::unordered_map<int, Texture> m_cache;
		std::unordered_map<int, FontMap> m_glyphs;

		void* m_face = nullptr;
		void* m_lib = nullptr;
	};

	struct PURE2D_API Text : public Renderable
	{
		Font* parentFont;
		Vec4f color = Vec4f::single(1.f);
		Mesh mesh;

		friend Font;

		void setSize(Vec2f size);
		void setPosition(const Vec3f& pos);
		void setPosition(Vec2f pos);
		void setRotation(float rot);

		void move(const Vec3f& pos);
		void scale(Vec2f size);
		void rotate(float rot);

		// TODO: Handle newlines and potentially other whitespace characters.
		void setString(const char* text);

		const Transform& transform() const;

		void draw(Renderer& renderer) final;

	private:

		void updateVerts();

		std::string m_textString;
		std::vector<Quad> m_quads;
		float m_characterSize;

		Transform m_transform;
		bool m_needsUpdate;

		void updateColorUniform();
	};
}


#endif // PURE2D_GRAPHICS_FONT_H

