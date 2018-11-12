#if 1
#include "Font.h"
#include "Private/FreeType.h"
#include "External/stb_truetype.h"
#include <Pure2D/Graphics/Transform.h>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Quad.h> 
#include <Pure2D/Graphics/Mesh.h>
#include <Pure2D/Graphics/Renderer.h>

#include <unordered_map>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>

#define fttFace(handle) (static_cast<FT_Face>(handle))

using namespace pure;

static void moveGlyph(Vec2f pos, Glyph& g);

enum UniLocs
{
	PROJ_MAT_LOC = 0, COLOR_LOC
};

static struct FontShader
{
	Shader base;
	int m_uniformLocs[2];

	static FontShader& defaultInstance()
	{
		static FontShader fs{};
		return fs;
	}

private:
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
		"	//if (texture(tex, texCoord).r == 0.0) discard;\n"
		"   //return vec4(texture(tex, texCoord).r, texture(tex, texCoord).r, texture(tex, texCoord).r, texture(tex, texCoord).r);\n"
		"    return vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r) * u_color;\n"
		"   //return vec4(1.0, 1.0, 1.0, 0.5);\n"
		"}\n";

	static const size_t defaultVertShaderlen;
	static const size_t defaultFragShaderlen;

	FontShader()
	{
		std::string v{};
		std::string f{};

		v.resize(FontShader::defaultVertShaderlen * 2);
		f.resize(FontShader::defaultFragShaderlen * 2);

		Shader::createVertShader(&v[0], FontShader::defaultVert, false);
		Shader::createFragShader(&f[0], FontShader::defaultFrag);

		base = Shader::createSrc(v.c_str(), f.c_str());
		m_uniformLocs[COLOR_LOC] = base.getLocation("u_color");
		m_uniformLocs[PROJ_MAT_LOC] = base.getLocation("u_projMatrix");
	}

	void setColor(const Vec4f& m_color) const
	{
		base.setUniform(m_uniformLocs[COLOR_LOC], m_color);
	}

	void setProjMat(const Mat4& projMat) const
	{
		base.setUniform(m_uniformLocs[PROJ_MAT_LOC], projMat);
	}
};

const size_t FontShader::defaultVertShaderlen = Shader::getVertShaderSize(strlen(FontShader::defaultVert));
const size_t FontShader::defaultFragShaderlen = Shader::getFragShaderSize(strlen(FontShader::defaultFrag));

void pure::Text::updateVerts()
{
	//Font::FontMap& fm = m_parentFont->m_glyphs[m_characterSize];

	//Vec3f pos = m_transform.position();
	//
	//for (size_t i = 0; i < m_textString.size(); i++)
	//{
	//	char c = m_textString[i];
	//	Quad& q = m_quads[i];
	//	const Glyph& g = fm.glyphs[c];

	//	Transform t = m_transform;

	//	t.setPosition({ pos.x + g.bitmap.x, pos.y + g.bitmap.y });
	//	q.translate(t.modelMatrix());

	//	pos += g.advance;
	//}

	//const auto* verts = reinterpret_cast<Vertex2D*>(&m_quads[0]);
	//const size_t numVerts = m_quads.size() * Quad::VERT_COUNT;
	//m_mesh.vbo.writeBuffer(verts, numVerts, 0);

}

void pure::Text::setSize(Vec2f size)
{
	m_transform.setSize(size);
	m_needsUpdate = true;
}

void pure::Text::setPosition(Vec3f pos)
{
	m_transform.setPosition(pos);
	m_needsUpdate = true;
}

void pure::Text::setRotation(float rot)
{
	m_transform.setRotation(rot);
	m_needsUpdate = true;
}

void pure::Text::setColor(const Vec4<float>& m_color)
{
	this->m_color = m_color;
}

const Transform & pure::Text::transform() const
{
	return m_transform;
}
void pure::Text::draw(Renderer & renderer)
{
	if (m_needsUpdate)
		updateVerts();

	m_mesh.shader.setUniform(m_uniformLocs[PROJ_MAT_LOC], renderer.projection());
	updateColorUniform();
	renderer.drawMeshStatic(m_mesh);
}

void pure::Font::free()
{
	clear();
}


bool pure::Font::loadFromFile(const char * filename)
{
	clear();

	if (FT_New_Face(FreeType::get().lib, filename, 0, reinterpret_cast<FT_Face*>(&m_face)))
	{
		std::cerr << "Could not open font: " << filename << std::endl;
		return false;
	}

	return true;
}

void pure::Text::updateColorUniform()
{
	m_mesh.shader.setUniform(m_uniformLocs[COLOR_LOC], m_color);
}

void pure::Font::loadSize(int size)
{
	//// if we already have glyphs for this size we don't need to do anything else.
	//auto itr = m_glyphs.insert(std::make_pair(size, FontMap{}));
	//if (!itr.second) return;
	//
	//FontMap& fm = itr.first->second;

	//Texture::setAlignment(1);

	//FT_GlyphSlot g = fttFace(m_face)->glyph;

	//FT_Set_Pixel_Sizes(fttFace(m_face), 0, size);

	//int w = 0, h = 0;
	//for (size_t i = 0; i < MAX_NUM_CHARS; i++)
	//{
	//	if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
	//	{
	//		std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
	//	}

	//	FT_GlyphSlot g = fttFace(m_face)->glyph;
	//	w += g->bitmap.width;
	//	h = std::max(h, (int)g->bitmap.rows);
	//}

	//Texture& texture = m_cache.insert(
	//	std::make_pair(size, Texture::create(w, h, Texture::Format::RED, Texture::Format::RED, nullptr))
	//).first->second;

	//int offset = 0;

	//for (size_t i = 0; i < MAX_NUM_CHARS; i++)
	//{
	//	if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
	//	{
	//		std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
	//	}

	//	FT_GlyphSlot g = fttFace(m_face)->glyph;

	//	fm.glyphs[i].xOffsetCoord = float(offset);
	//	fm.glyphs[i].bitmap = {
	//		float(g->bitmap_left),
	//		float(g->bitmap_top),
	//		float(g->bitmap.width),
	//		float(g->bitmap.rows)
	//	};
	//	fm.glyphs[i].advance = {
	//		g->advance.x >> 6,
	//		g->advance.y >> 6
	//	};

	//	texture.write({ offset, 0 }, g->bitmap.width, g->bitmap.rows, Texture::Format::RED, DataType::UBYTE, g->bitmap.buffer);
	//	offset += g->bitmap.width + 1;
	//}

	/*uint32_t w = 0;
	uint32_t h = 0;

	FT_Set_Pixel_Sizes(fttFace(m_face), 0, size);

	for (int i = 0; i < MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
		{
			std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
			continue;
		}

		w += g->bitmap.width;
		h = std::max(h, g->bitmap.rows);
	}

	fm.height = h;
	fm.width = w;

	Texture& tex = m_cache.insert(
		std::make_pair(size, Texture::create(w, h, Texture::Format::RED, Texture::Format::RED, nullptr))
	).first->second;

	uint32_t x = 0;

	for (int i = 0; i < MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
			continue;

		tex.write({ int(x), 0 }, g->bitmap.width, g->bitmap.rows, Texture::Format::RED, DataType::UBYTE, g->bitmap.buffer);

		fm.m_glyphs[i].advance.x = g->advance.x >> 6;
		fm.m_glyphs[i].advance.y = g->advance.y >> 6;

		fm.m_glyphs[i].bitmap.w = g->bitmap.width;
		fm.m_glyphs[i].bitmap.h = g->bitmap.rows;

		fm.m_glyphs[i].bitmap.x = g->bitmap_left;
		fm.m_glyphs[i].bitmap.y = g->bitmap_top;

		fm.m_glyphs[i].xOffsetCoord = static_cast<float>(x) / w;

		x += g->bitmap.width + 1;
	}*/
	
	Texture::setAlignment(4);
}

pure::Text pure::Font::makeText(const char * text, int size)
{
	Texture::setAlignment(1);
	FT_Set_Pixel_Sizes(fttFace(m_face), 0, size);

	const size_t texLen = strlen(text);

	int w = 0, h = 0;
	for (size_t i = 0; i < MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
		{
			std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
		}

		FT_GlyphSlot g = fttFace(m_face)->glyph;
		w += g->bitmap.width;
		h = std::max(h, (int)g->bitmap.rows);
	}

	Texture texture = Texture::create(w, h, Texture::Format::RED, Texture::Format::RED, nullptr);

	int offset = 0;

	struct {
		int offset;
		int w, h;
		float advance;
		int x, y;
	} characters[MAX_NUM_CHARS];

	for (size_t i = 0; i < MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(fttFace(m_face), i, FT_LOAD_RENDER))
		{
			std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
		}		

		FT_GlyphSlot g = fttFace(m_face)->glyph;

		characters[i].offset = offset;
		characters[i].w = g->bitmap.width;
		characters[i].h = g->bitmap.rows;
		characters[i].advance = g->advance.x >> 6;
		characters[i].x = g->bitmap_left;
		characters[i].y = g->bitmap_top;

		texture.write({ offset, 0 }, g->bitmap.width, g->bitmap.rows, Texture::Format::RED, DataType::UBYTE, g->bitmap.buffer);
		offset += g->bitmap.width;
	}

	m_cache[size] = texture;
	Texture::setAlignment(4);

	Text tex = {};

	tex.m_parentFont = this;
	tex.m_textString = text;
	tex.m_characterSize = size;

	tex.m_mesh.vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), strlen(text) * 2 * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);
	tex.m_mesh.ebo = ElementBuffer::quad(6 * strlen(text) * 2);
	tex.m_mesh.primtype = DrawPrimitive::TRIANGLES;
	tex.m_mesh.texture = &m_cache[size];

	tex.m_mesh.shader = FontShader::defaultInstance().base;
	memcpy_s(tex.m_uniformLocs, sizeof(tex.m_uniformLocs), FontShader::defaultInstance().m_uniformLocs, sizeof(FontShader::defaultInstance().m_uniformLocs));
	tex.m_quads.reserve(1);

	std::vector<Quad> qs;
	float xPos = 50.f;
	const float yPos = 50.f;
	for (const char* c = text; *c; c++)
	{
		Transform t = Transform::create();
		Quad q = Quad::create();

		auto character = characters[*c];

		Rectui r = {
			uint32_t(character.offset),
			h - character.h,
			uint32_t(character.w),
			uint32_t(character.h)
		};
		q.setTextureCoords(r, { w, h });

		q.flipVerticalTexCoords(0.f, (float)character.h / (float)h);

		t.setPosition({ character.x + xPos, (yPos + (character.h - character.y)) - character.h, 50.0f });
		t.setSize({ (float)character.w, (float)character.h });
		q.translate(t.modelMatrix());

		if (*(c + 1))
		{
			FT_Vector delta;
			FT_Get_Kerning(fttFace(m_face), *c, *(c + 1), FT_KERNING_DEFAULT, &delta);

			xPos += character.advance + (delta.x >> 6);

		}
		qs.push_back(q);
	}

	tex.m_mesh.vbo.writeBuffer(reinterpret_cast<Vertex2D*>(&qs[0]), qs.size() * Quad::VERT_COUNT, 0);
	return tex;
}

void pure::Font::clear()
{
	for (auto& kv : m_cache)
		kv.second.free();

	FT_Done_Face(fttFace(m_face));
	m_face = nullptr;
}


#else
#include "Font.h"
#include "Private/FileIO.h"
#include "External/stb_truetype.h"
#include <Pure2D/Graphics/Transform.h>
#include <Pure2D/Graphics/Texture.h>
#include <Pure2D/Graphics/Quad.h> 
#include <Pure2D/Graphics/Mesh.h>
#include <Pure2D/Graphics/Renderer.h>

#include "External/glad.h"

#include <unordered_map>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace pure;

static constexpr int MAX_NUM_CHARS = 128;

// TODO: FUCK THIS SHIT USE STB_TRUETYPE FOOLIO

enum UniLocs
{
	PROJ_MAT_LOC = 0, COLOR_LOC
};

static struct FontMap
{
	Glyph glyphs[MAX_NUM_CHARS] = {};
	float height = 0.f;
	float width = 0.f;
};

static struct FontShader
{
	Shader base;
	int uniformLocs[2];

	static FontShader& defaultInstance()
	{
		static FontShader fs{};
		return fs;
	}

private:
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
		"   //return vec4(texture(tex, texCoord).r, texture(tex, texCoord).r, texture(tex, texCoord).r, 1.0);\n"
		"    return vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r) * u_color;\n"
		"   //return vec4(1.0, 1.0, 1.0, 1.0);\n"
		"}\n";

	static const size_t defaultVertShaderlen;
	static const size_t defaultFragShaderlen;

	FontShader()
	{
		std::string v{};
		std::string f{};

		v.resize(FontShader::defaultVertShaderlen * 2);
		f.resize(FontShader::defaultFragShaderlen * 2);

		Shader::createVertShader(&v[0], FontShader::defaultVert, false);
		Shader::createFragShader(&f[0], FontShader::defaultFrag);

		base = Shader::createSrc(v.c_str(), f.c_str());
		uniformLocs[COLOR_LOC] = base.getLocation("u_color");
		uniformLocs[PROJ_MAT_LOC] = base.getLocation("u_projMatrix");
	}

	void setColor(const Vec4f& color) const
	{
		base.setUniform(uniformLocs[COLOR_LOC], color);
	}

	void setProjMat(const Mat4& projMat) const
	{
		base.setUniform(uniformLocs[PROJ_MAT_LOC], projMat);
	}
};

const size_t FontShader::defaultVertShaderlen = Shader::getVertShaderSize(strlen(FontShader::defaultVert));
const size_t FontShader::defaultFragShaderlen = Shader::getFragShaderSize(strlen(FontShader::defaultFrag));

namespace pure
{
	struct Font_Impl
	{
		// fonts cached with thier heights as keys
		std::unordered_map<int, Texture> cache;
		std::unordered_map<int, FontMap> glyphs;

		stbtt_fontinfo fontInfo;
		std::string fontBuff;

		void clear()
		{
			for (auto& kv : cache)
				kv.second.free();
		}
	};

	struct Text_Impl
	{
		Font* parentFont;
		float characterSize;
		std::string textString;
		std::vector<Quad> quads;
		Mesh mesh;
		Transform transform;
		Vec4f color = Vec4f::single(1.f);
		bool needsUpdate;

		int uniformLocs[2];

		void updateColorUniform()
		{
			mesh.shader.setUniform(uniformLocs[COLOR_LOC], color);
		}
	};
}

Text::Text() : m_impl(new Text_Impl())
{ }

Text::Text(const Text& other)
{
	m_impl = new Text_Impl(*other.m_impl);
}

pure::Text::Text(Text && other) noexcept:
m_impl(new Text_Impl(std::move(*other.m_impl)))
{
	other.parentFont = nullptr;
	other.mesh.texture = nullptr;
}

void pure::Text::updateVerts()
{
	//auto* self = m_impl;
	//FontMap& fm = self->parentFont->glyphs[self->characterSize];

	//Vec3f pos = self->transform.position();
	//
	//for (size_t i = 0; i < self->textString.size(); i++)
	//{
	//	char c = self->textString[i];
	//	Quad& q = self->quads[i];
	//	const Glyph& g = fm.glyphs[c];

	//	Transform t = self->transform;

	//	t.setPosition({ pos.x + g.bitmap.x, pos.y + g.bitmap.y });
	//	q.translate(t.modelMatrix());

	//	pos += g.advance;
	//}

	//const auto* verts = reinterpret_cast<Vertex2D*>(&quads[0]);
	//const size_t numVerts = quads.size() * Quad::VERT_COUNT;
	//self->mesh.vbo.writeBuffer(verts, numVerts, 0);

}

Text::~Text()
{
	delete m_impl;
	m_impl = nullptr;
}

void pure::Text::setSize(Vec2f size)
{
	transform.setSize(size);
	needsUpdate = true;
}

void pure::Text::setPosition(Vec3f pos)
{
	transform.setPosition(pos);
	needsUpdate = true;
}

void pure::Text::setRotation(float rot)
{
	transform.setRotation(rot);
	needsUpdate = true;
}

void pure::Text::setColor(const Vec4<float>& color)
{
	color = color;
}

const Transform & pure::Text::transform() const
{
	return transform;
}

void pure::Text::draw(Renderer & renderer)
{
	if (needsUpdate)
		updateVerts();

	mesh.shader.setUniform(uniformLocs[PROJ_MAT_LOC], renderer.projection());
	updateColorUniform();
	renderer.drawMeshStatic(mesh);
}

Font::Font() : m_impl(new Font_Impl())
{ }

Font::~Font()
{
	delete m_impl;
	m_impl = nullptr;
}

void pure::Font::free()
{
	clear();
}

#include <cstring>
bool pure::Font::loadFromFile(const char * filename)
{
	clear();

	fontBuff = readFile(filename);
	auto* buff = reinterpret_cast<const uint8_t*>(fontBuff.c_str());

	if (!stbtt_InitFont(&fontInfo, buff, stbtt_GetFontOffsetForIndex(buff, 0)))
	{
		std::cout << "Failed to load font: " << filename << std::endl;
		return false;
	}

	return true;
}

void pure::Font::loadSize(int size)
{
	//// if we already have glyphs for this size we don't need to do anything else.
	//auto itr = glyphs.insert(std::make_pair(size, FontMap{}));
	//if (!itr.second) return;
	//
	//FontMap& fm = itr.first->second;

	//Texture::setAlignment(1);

	//FT_GlyphSlot g = face->glyph;

	//uint32_t w = 0;
	//uint32_t h = 0;

	//FT_Set_Pixel_Sizes(face, 0, size);

	//for (int i = 0; i < MAX_NUM_CHARS; i++)
	//{
	//	if (FT_Load_Char(face, i, FT_LOAD_RENDER))
	//	{
	//		std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
	//		continue;
	//	}

	//	w += g->bitmap.width;
	//	h = std::max(h, g->bitmap.rows);
	//}

	//fm.height = h;
	//fm.width = w;

	//Texture& tex = cache.insert(
	//	std::make_pair(size, Texture::create(w, h, Texture::Format::RED, Texture::Format::RED, nullptr))
	//).first->second;

	//uint32_t x = 0;

	//for (int i = 0; i < MAX_NUM_CHARS; i++)
	//{
	//	if (FT_Load_Char(face, i, FT_LOAD_RENDER))
	//		continue;

	//	tex.write({ int(x), 0 }, g->bitmap.width, g->bitmap.rows, Texture::Format::RED, DataType::UBYTE, g->bitmap.buffer);

	//	fm.glyphs[i].advance.x = g->advance.x >> 6;
	//	fm.glyphs[i].advance.y = g->advance.y >> 6;

	//	fm.glyphs[i].bitmap.w = g->bitmap.width;
	//	fm.glyphs[i].bitmap.h = g->bitmap.rows;

	//	fm.glyphs[i].bitmap.x = g->bitmap_left;
	//	fm.glyphs[i].bitmap.y = g->bitmap_top;

	//	fm.glyphs[i].xOffsetCoord = static_cast<float>(x) / w;

	//	x += g->bitmap.width + 1;
	//}
	//
	//Texture::setAlignment(4);
}

pure::Text pure::Font::makeText(const char * text, int size)
{
	int w, h;
	uint8_t* bitmap = stbtt_GetCodepointBitmap(&fontInfo, 0,
		stbtt_ScaleForPixelHeight(&fontInfo, size), 'G', &w, &h, 0, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	cache[size] = Texture::create(w, h, Texture::Format::RED, Texture::Format::RED, bitmap);


	Text tex = {};

	tex.parentFont = this;
	tex.textString = text;
	tex.characterSize = size;

	tex.mesh = Mesh::quad(DrawUsage::DYNAMIC_DRAW);
	tex.mesh.texture = &cache[size];

	tex.mesh.shader = FontShader::defaultInstance().base;
	memcpy_s(tex.uniformLocs, sizeof(tex.uniformLocs), FontShader::defaultInstance().uniformLocs, sizeof(FontShader::defaultInstance().uniformLocs));
	tex.quads.reserve(1);

	Transform t = Transform::create();
	Quad q = Quad::create();

	q.flipVerticalTexCoords();

	t.setPosition({ 50.f, 50.f, -1.0f });
	t.setSize({ 50.f, 50.f });
	q.translate(t.modelMatrix());
	tex.mesh.vbo.writeBuffer(q.verts, Quad::VERT_COUNT, 0);

	return tex;
}

#endif

