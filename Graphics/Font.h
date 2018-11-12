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

		void* m_face;

		void clear();
	};

	struct PURE2D_API Text : public Renderable
	{
		friend Font;

		void setSize(Vec2f size);
		void setPosition(Vec3f pos);
		void setRotation(float rot);
		void setColor(const Vec4<float>& m_color);

		const Transform& transform() const;

		void draw(Renderer& renderer) final;

	private:

		void updateVerts();

		Font* m_parentFont;
		float m_characterSize;
		std::string m_textString;
		std::vector<Quad> m_quads;
		Mesh m_mesh;
		Transform m_transform;
		Vec4f m_color = Vec4f::single(1.f);
		bool m_needsUpdate;

		int m_uniformLocs[2];

		void updateColorUniform();
	};
}


#endif // PURE2D_GRAPHICS_FONT_H

