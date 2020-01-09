
#include "Font.h"
#include "External/stb_truetype.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <Pure2D/Graphics/Transform.h>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Quad.h> 
#include <Pure2D/Graphics/Mesh.h>
#include <Pure2D/Graphics/Renderer.h>
#include "Private/Util.h"

#include <unordered_map>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <limits>

#define fttFace(handle) (static_cast<FT_Face>(handle))
#define fttLib(handle) (static_cast<FT_Library>(handle))

using namespace pure;

static inline void setGlyphTransform(Transform& t, const Vec3f& pos, const Glyph& g);
static inline void setGlyphTexCoords(const Glyph& g, const FontMap& fm, Quad& q);
static void resizeBufferDbl(Mesh& mesh, const std::vector<Quad>& quads);
static void writeQuadsToBuffer(Mesh& mesh, std::vector<Quad>& quads);
static inline void updateFontShaderColor(const Shader& shader, const Vec4f& color);

struct FontShader
{

	Shader base;

	enum
	{
		PROJ_MAT = Shader::DEFAULT_LOC_COUNT,
		COLOR
	};

	static FontShader& defaultInstance()
	{
		static FontShader fs{};
		return fs;
	}

private:
	// TODO/NOTE: Should continue to force text to be drawn in view space?
	// or should we allow it to be drawn in world coordinates?
	// if so, we can either allow passing a custom shader for the text (need to handle deleting old shader)
	// or we can always multiply by view matrix to draw in world space and just make user handle positioning.
	// OR both, because why tf not lmao
	static constexpr const char* defaultVert = "\n"
		"uniform mat4 u_projMatrix;\n"
		"vec4 position(mat4 mvpMat, vec3 pos)\n"
		"{\n"
		"   FragPos = pos;"
		"   return u_projMatrix * vec4(pos, 1.0);\n"
		"}\n";

	static constexpr const char* defaultFrag = "\n"
		"uniform vec4 u_color;"

		"vec4 effect(vec4 color, sampler2D tex, vec2 texCoord, vec3 fragPos)\n"
		"{\n"
		"    return vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r) * u_color;\n"
		"}\n";

	FontShader()
	{
		std::string v{};
		std::string f{};

		//v.resize(FontShader::defaultVertShaderlen * 2);
		//f.resize(FontShader::defaultFragShaderlen * 2);

		//Shader::createVertShader(&v[0], FontShader::defaultVert, false);
		//Shader::createFragShader(&f[0], FontShader::defaultFrag);

		base = Shader::fromTemplate(FontShader::defaultVert, FontShader::defaultFrag);
		base.locations.resize(base.locations.size() + 2);
		base.locations[COLOR] = base.getLocation("u_color");
		base.locations[PROJ_MAT] = base.getLocation("u_projMatrix");
	}

	void setColor(const Vec4f& color) const
	{
		base.setUniformIndx(COLOR, color);
	}

	void setProjMat(const Mat4& projMat) const
	{
		base.setUniformIndx(PROJ_MAT, projMat);
	}
};

const char * pure::Text::string() const
{
	return m_textString.c_str();
}

const Rectf & pure::Text::bounds() const
{
	return m_bounds;
}

void pure::Text::setSize(Vec2f size)
{
	m_transform.setSize(size);
	m_needsUpdate = true;
}

void pure::Text::setPosition(const Vec3f& pos)
{
	m_transform.setPosition(pos);
	m_needsUpdate = true;
}

void pure::Text::setPosition(Vec2f pos)
{
	m_transform.setPosition({pos.x, pos.y, m_transform.position().z });
	m_needsUpdate = true;
}

void pure::Text::setRotation(float rot)
{
	m_transform.setRotation(rot);
	m_needsUpdate = true;
}

void pure::Text::move(const Vec3f & pos)
{
	m_transform.setPosition(m_transform.position() + pos);
	m_needsUpdate = true;
}

void pure::Text::scale(Vec2f size)
{
	m_transform.setSize(m_transform.size() + size);
	m_needsUpdate = true;
}

void pure::Text::rotate(float rot)
{
	m_transform.setRotation(m_transform.rotation() + rot);
	m_needsUpdate = true;
}

void pure::Text::resize(size_t texLen)
{
	if (texLen <= m_textString.size()) return;

	m_quads.resize(texLen);
	m_textString.resize(texLen, ' ');

	if (m_quads.size() * sizeof(Quad) >= m_mesh.vbo.size)
		resizeBufferDbl(m_mesh, m_quads);
}

void pure::Text::setString(const char * text)
{
	const FontMap* fm = m_parentFont->getMap(m_characterSize);

	Vec3f pos = m_transform.position();

	m_quads.clear();

	Vec2f min = Vec2f::single(std::numeric_limits<float>::max());
	Vec2f max = {};

	for (const char* c = text; *c; c++)
	{
		Transform t = Transform::make();
		Quad q = Quad::make();

		const Glyph* g = &fm->glyphs[*c];

		// TODO: Need some representation of line-spacing 
		if (*c == '\n')
		{
			pos.y += fm->height;
			pos.x = m_transform.position().x;

			g = &fm->glyphs[' '];
		}

		setGlyphTexCoords(*g, *fm, q);

		setGlyphTransform(t, { pos.x, pos.y, pos.y }, *g);
		t.setSize({ g->bitmap.w, g->bitmap.h });
		q.translate(t.modelMatrix());

		min = {
			std::min(min.x, q.verts[0].position.x),
			std::min(min.y, q.verts[0].position.y)
		};
		max = {
			std::max(max.x, q.verts[Quad::VERT_COUNT - 1].position.x),
			std::max(max.y, q.verts[Quad::VERT_COUNT - 1].position.y)
		};

		if (*(c + 1))
		{
			Vec2i kerning = m_parentFont->getKerning(*c, *(c + 1));
			pos.x += g->advance.x + (kerning.x >> 6);
		}

		m_quads.push_back(q);
	}

	writeQuadsToBuffer(m_mesh, m_quads);

	{
		const auto& firstPos = m_quads.front().verts[0].position;
		const auto& lastPos = m_quads.back().verts[Quad::VERT_COUNT - 1].position;

		m_bounds.x = min.x;
		m_bounds.y = min.y;
		m_bounds.w = max.x - min.x;
		m_bounds.h = max.y - min.y;
	}

	m_textString = text;
}

void pure::Text::setSubString(const char * text, size_t length, size_t index)
{
	vec_bounds_assert(m_textString, length);
	vec_bounds_assert(m_textString, index);
	vec_bounds_assert(m_textString, index + length);

	const char* tItr = text;
	const size_t writeCount = index + length;
	const FontMap* fm = m_parentFont->getMap(m_characterSize);

	for (size_t i = index; i < writeCount; i++)
	{
		m_textString[i] = *tItr;

		const Glyph& g = fm->glyphs[*tItr];

		Quad& q = m_quads[i];
		setGlyphTexCoords(g, *fm, q);

		tItr++;
	}

	const auto verts = reinterpret_cast<Vertex2D*>(&m_quads[index]);
	m_mesh.vbo.writeBuffer(verts, Quad::VERT_COUNT * length, sizeof(Quad) * index);
}

void pure::Text::setChar(char c, size_t index)
{
	vec_bounds_assert(m_textString, index);
	m_textString[index] = c;

	const FontMap* fm = m_parentFont->getMap(m_characterSize);
	const Glyph& g = fm->glyphs[c];
	Quad& q = m_quads[index];
	setGlyphTexCoords(g, *fm, q);

	m_mesh.vbo.writeBuffer(q.verts, Quad::VERT_COUNT, sizeof(Quad) * index);
}

const Transform & pure::Text::transform() const
{
	return m_transform;
}

void pure::Text::draw(Renderer & renderer)
{
	if (m_needsUpdate)
		updateVerts();

	m_mesh.shader.setUniformIndx(FontShader::PROJ_MAT, renderer.projection());
	updateFontShaderColor(m_mesh.shader, color);
	renderer.drawMeshStatic(m_mesh);
}

void pure::Font::free()
{
	for (auto& kv : m_cache)
		kv.second.free();

	FT_Done_Face(fttFace(m_face));
	m_face = nullptr;

	FT_Done_FreeType(fttLib(m_lib));
	m_lib = nullptr;
}

bool pure::Font::loadFromFile(const char * filename)
{
	free();

	if (FT_Init_FreeType(reinterpret_cast<FT_Library*>(&m_lib)))
	{
		std::cerr << "Could not init FreeType" << std::endl;
		return false;
	}


	if (FT_New_Face(fttLib(m_lib), filename, 0, reinterpret_cast<FT_Face*>(&m_face)))
	{
		std::cerr << "Could not open font: " << filename << std::endl;
		return false;
	}

	return true;
}

void pure::Text::updateVerts()
{
	const FontMap* fm = m_parentFont->getMap(m_characterSize);

	Vec3f pos = m_transform.position();

	for (size_t i = 0; i < m_textString.size(); i++)
	{
		const char* c = m_textString.c_str() + i;

		if (*c == '\n')
		{
			pos.y += fm->height;
			pos.x = m_transform.position().x;
			m_quads.push_back(Quad{});
			continue;
		}

		Transform t = Transform::make();
		Quad& q = m_quads[i];

		const Glyph& g = fm->glyphs[*c];

		setGlyphTransform(t, pos, g);
		t.setSize({ m_transform.size().x + g.bitmap.w, m_transform.size().y + g.bitmap.h });
		t.setRotation(m_transform.rotation());
		q.translate(t.modelMatrix());

		if (*(c + 1))
		{
			Vec2i kerning = m_parentFont->getKerning(*c, *(c + 1));
			pos.x += g.advance.x + (kerning.x >> 6);
		}
	}

	writeQuadsToBuffer(m_mesh, m_quads);
	m_needsUpdate = false;
}

void pure::Font::loadSize(int size)
{
	auto itr = m_glyphs.insert(std::make_pair(size, FontMap{}));
	if (!itr.second) return;

	FontMap& fm = itr.first->second;

	Texture::setAlignment(1);

	FT_Set_Pixel_Sizes(fttFace(m_face), 0, size);

	for (size_t i = 0; i < FONT_MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
		{
			std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
		}

		FT_GlyphSlot g = fttFace(m_face)->glyph;
		fm.width += g->bitmap.width;
		fm.height = std::max(fm.height, float(g->bitmap.rows));
	}

	Texture texture = Texture::make(fm.width, fm.height, Texture::Format::RED, Texture::Format::RED, nullptr);


	int offset = 0;

	for (size_t i = 0; i < FONT_MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
		{
			std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
		}

		FT_GlyphSlot g = fttFace(m_face)->glyph;

		fm.glyphs[i].xOffsetCoord = static_cast<float>(offset);
		fm.glyphs[i].bitmap.w = static_cast<float>(g->bitmap.width);
		fm.glyphs[i].bitmap.h = static_cast<float>(g->bitmap.rows);
		fm.glyphs[i].bitmap.x = static_cast<float>(g->bitmap_left);
		fm.glyphs[i].bitmap.y = static_cast<float>(g->bitmap_top);
		fm.glyphs[i].advance.x = g->advance.x >> 6;
		fm.glyphs[i].advance.y = g->advance.y >> 6;

		texture.write({ offset, 0 }, g->bitmap.width, g->bitmap.rows, Texture::Format::RED, DataType::UBYTE, g->bitmap.buffer);
		offset += g->bitmap.width;
	}

	m_cache[size] = texture;

	Texture::setAlignment(4);
}

pure::Text pure::Font::makeText(const char * text, int size)
{
	auto itr = m_glyphs.find(size);
	if (itr == m_glyphs.end())
	{
		loadSize(size);
		itr = m_glyphs.find(size);
	}

	const FontMap& fm = itr->second;
	const size_t texLen = strlen(text);
	const size_t reserveSpace = texLen * 2;

	Text tex = {};

	tex.m_parentFont = this;
	tex.m_characterSize = size;
	tex.move({ 0.f, 0.f, Renderer::clipFar() });

	tex.m_mesh.vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), reserveSpace * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);
	tex.m_mesh.ebo = ElementBuffer::quad(6 * reserveSpace);
	tex.m_mesh.primtype = DrawPrimitive::TRIANGLES;
	tex.m_mesh.texture = &m_cache[size];

	tex.m_mesh.shader = FontShader::defaultInstance().base;
	tex.m_quads.reserve(reserveSpace);

	tex.setString(text);
	return tex;
}

Vec2i pure::Font::getKerning(char left, char right) const
{
	FT_Vector delta;
	FT_Get_Kerning(fttFace(m_face), left, right, FT_KERNING_DEFAULT, &delta);
	return {
		delta.x,
		delta.y
	};
}

const FontMap * pure::Font::getMap(int size) const
{
	return mapGet(m_glyphs, size);
}

const Texture * pure::Font::getTexture(int size) const
{
	return mapGet(m_cache, size);
}

void setGlyphTransform(Transform& t, const Vec3f& pos, const Glyph& g)
{
	t.setPosition({ g.bitmap.x + pos.x, (pos.y + (g.bitmap.h - g.bitmap.y)) - g.bitmap.h, pos.z });
}

void setGlyphTexCoords(const Glyph& g, const FontMap& fm, Quad& q)
{
	Rectui r = {
		uint32_t(g.xOffsetCoord),
		uint32_t(fm.height - g.bitmap.h),
		uint32_t(g.bitmap.w),
		uint32_t(g.bitmap.h)
	};

	q.setTextureCoords(r, { int(fm.width), int(fm.height) });

	q.flipVerticalTexCoords(0.f, g.bitmap.h / fm.height);
}

void resizeBufferDbl(Mesh& mesh, const std::vector<Quad>& quads)
{
	const auto* verts = reinterpret_cast<const Vertex2D*>(&quads[0]);
	const size_t vertCount = quads.size() * Quad::VERT_COUNT;

	const size_t reserveSpace = quads.size() * 2;

	mesh.vbo.alloc((Vertex2D*)0, reserveSpace * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW);
	mesh.vbo.writeBuffer(verts, vertCount, 0);
	mesh.ebo.free();

	mesh.ebo = ElementBuffer::quad(6 * reserveSpace);
}

void writeQuadsToBuffer(Mesh& mesh, std::vector<Quad>& quads)
{
	const auto* verts = reinterpret_cast<Vertex2D*>(&quads[0]);
	const size_t vertCount = quads.size() * Quad::VERT_COUNT;

	if (quads.size() * sizeof(Quad) >= mesh.vbo.size)
	{
		resizeBufferDbl(mesh, quads);
		quads.reserve(quads.size() * 2);
	}
	else
	{
		mesh.vbo.writeBuffer(verts, vertCount, 0);
	}
}

void updateFontShaderColor(const Shader& shader, const Vec4f& color)
{
	shader.setUniformIndx(FontShader::COLOR, color);
}
