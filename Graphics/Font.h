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

	constexpr int FONT_MAX_NUM_CHARS = 128;
	struct FontMap
	{
		Glyph glyphs[FONT_MAX_NUM_CHARS] = {};
		float height = 0.f;
		float width = 0.f;
	};

	struct PURE2D_API Font : private NonCopyable
	{
		void free();

		bool loadFromFile(const char* filename);
		void loadSize(int size);

		Text makeText(const char* text, int size);

		Vec2i getKerning(char left, char right) const;
		const FontMap* getMap(int size) const;
		const Texture* getTexture(int size) const;

	private:

		// fonts cached with thier heights as keys
		std::unordered_map<int, Texture> m_cache;
		std::unordered_map<int, FontMap> m_glyphs;

		void* m_face = nullptr;
		void* m_lib = nullptr;
	};

	// TODO: Implement getting the text bounding box 
	// once we have incorporated line height, padding, spacing, ect.
	struct PURE2D_API Text : public Renderable
	{
		Vec4f color = Vec4f::single(1.f);

		friend Font;

		const char* string() const;
		const Rectf& bounds() const;

		void setSize(Vec2f size);
		void setPosition(const Vec3f& pos);
		void setPosition(Vec2f pos);
		void setRotation(float rot);

		void move(const Vec3f& pos);
		void scale(Vec2f size);
		void rotate(float rot);

		void resize(size_t texLen);

		// TODO: Handle newlines and potentially other whitespace characters.
		void setString(const char* text);

		void setSubString(const char* text, size_t length, size_t index);
		void setChar(char c, size_t index);

		const Transform& transform() const;

		void draw(Renderer& renderer) final;

	private:

		std::string m_textString;
		std::vector<Quad> m_quads;
		float m_characterSize;

		Mesh m_mesh;
		const Font* m_parentFont;
		Rectf m_bounds;

		Transform m_transform;
		bool m_needsUpdate;

		void updateVerts();
	};
}


#endif // PURE2D_GRAPHICS_FONT_H

