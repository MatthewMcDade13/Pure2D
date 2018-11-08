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
		"   //return vec4(1.0, 1.0, 1.0, 1.0) * u_color;\n"
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

		FT_Face face;

		void clear()
		{
			for (auto& kv : cache)
				kv.second.free();

			FT_Done_Face(face);
			face = nullptr;
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

Font::Text::Text(): m_impl(new Text_Impl())
{ }

Font::Text::Text(const Text& other)
{
	m_impl = new Text_Impl(*other.m_impl);
}

pure::Font::Text::Text(Text && other) noexcept: 
	m_impl(new Text_Impl(std::move(*other.m_impl)))
{
	other.m_impl->parentFont = nullptr;
	other.m_impl->mesh.texture = nullptr;
}

void pure::Font::Text::updateVerts()
{
	auto* self = m_impl;
	FontMap& fm = self->parentFont->m_impl->glyphs[self->characterSize];

	Vec3f pos = self->transform.position();
	
	for (size_t i = 0; i < self->textString.size(); i++)
	{
		char c = self->textString[i];
		Quad& q = self->quads[i];
		const Glyph& g = fm.glyphs[c];

		Transform t = self->transform;

		t.setPosition({ pos.x + g.bitmap.x, pos.y + g.bitmap.y });
		q.translate(t.modelMatrix());

		pos += g.advance;
	}

	const auto* verts = reinterpret_cast<Vertex2D*>(&m_impl->quads[0]);
	const size_t numVerts = m_impl->quads.size() * Quad::VERT_COUNT;
	self->mesh.vbo.writeBuffer(verts, numVerts, 0);

}

Font::Text::~Text()
{
	delete m_impl;
	m_impl = nullptr;
}

void pure::Font::Text::setSize(Vec2f size)
{
	m_impl->transform.setSize(size);
	m_impl->needsUpdate = true;
}

void pure::Font::Text::setPosition(Vec3f pos)
{
	m_impl->transform.setPosition(pos);
	m_impl->needsUpdate = true;
}

void pure::Font::Text::setRotation(float rot)
{
	m_impl->transform.setRotation(rot);
	m_impl->needsUpdate = true;
}

void pure::Font::Text::setColor(const Vec4<float>& color)
{
	m_impl->color = color;
}

const Transform & pure::Font::Text::transform() const
{
	return m_impl->transform;
}

void pure::Font::Text::draw(Renderer & renderer)
{
	if (m_impl->needsUpdate)
		updateVerts();

	m_impl->mesh.shader.setUniform(m_impl->uniformLocs[PROJ_MAT_LOC], renderer.projection());
	m_impl->updateColorUniform();
	renderer.drawMeshStatic(m_impl->mesh);
}

Font::Font(): m_impl(new Font_Impl())
{ }

Font::~Font()
{
	delete m_impl;
	m_impl = nullptr;
}

void pure::Font::free()
{
	m_impl->clear();
}


bool pure::Font::loadFromFile(const char * filename)
{
	m_impl->clear();

	if (FT_New_Face(FreeType::get().lib, filename, 0, &m_impl->face))
	{
		std::cerr << "Could not open font: " << filename << std::endl;
		return false;
	}

	return true;
}

void pure::Font::loadSize(int size)
{
	// if we already have glyphs for this size we don't need to do anything else.
	auto itr = m_impl->glyphs.insert(std::make_pair(size, FontMap{}));
	if (!itr.second) return;
	
	FontMap& fm = itr.first->second;

	Texture::setAlignment(1);

	FT_GlyphSlot g = m_impl->face->glyph;

	uint32_t w = 0;
	uint32_t h = 0;

	FT_Set_Pixel_Sizes(m_impl->face, 0, size);

	for (int i = 0; i < MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(m_impl->face, i, FT_LOAD_RENDER))
		{
			std::cerr << "Loading character: " << char(i) << "failed." << std::endl;
			continue;
		}

		w += g->bitmap.width;
		h = std::max(h, g->bitmap.rows);
	}

	fm.height = h;
	fm.width = w;

	Texture& tex = m_impl->cache.insert(
		std::make_pair(size, Texture::create(w, h, Texture::Format::RED, Texture::Format::RED, nullptr))
	).first->second;

	uint32_t x = 0;

	for (int i = 0; i < MAX_NUM_CHARS; i++)
	{
		if (FT_Load_Char(m_impl->face, i, FT_LOAD_RENDER))
			continue;

		tex.write({ int(x), 0 }, g->bitmap.width, g->bitmap.rows, Texture::Format::RED, DataType::UBYTE, g->bitmap.buffer);

		fm.glyphs[i].advance.x = g->advance.x >> 6;
		fm.glyphs[i].advance.y = g->advance.y >> 6;

		fm.glyphs[i].bitmap.w = g->bitmap.width;
		fm.glyphs[i].bitmap.h = g->bitmap.rows;

		fm.glyphs[i].bitmap.x = g->bitmap_left;
		fm.glyphs[i].bitmap.y = g->bitmap_top;

		fm.glyphs[i].xOffsetCoord = static_cast<float>(x) / w;

		x += g->bitmap.width + 1;
	}
	
	Texture::setAlignment(4);
}

pure::Font::Text pure::Font::makeText(const char * text, int size)
{
	auto itr = m_impl->cache.find(size);
	if (itr == m_impl->cache.end())
	{
		loadSize(size);
		itr = m_impl->cache.find(size);
	}

	Texture& tex = itr->second;

	const size_t textLen = strlen(text);

	Text t = {};

	t.m_impl->parentFont = this;
	t.m_impl->textString = text;
	t.m_impl->characterSize = size;

	t.m_impl->mesh.ebo = ElementBuffer::quad(6 * textLen * 2);
	t.m_impl->mesh.vbo = VertexBuffer::createZeroed(sizeof(Vertex2D), textLen * 2 * Quad::VERT_COUNT, DrawUsage::DYNAMIC_DRAW, DataType::FLOAT);
	t.m_impl->mesh.primtype = DrawPrimitive::TRIANGLES;
	t.m_impl->mesh.texture = &tex;

	t.m_impl->mesh.shader = FontShader::defaultInstance().base;
	memcpy_s(t.m_impl->uniformLocs, sizeof(t.m_impl->uniformLocs), FontShader::defaultInstance().uniformLocs, sizeof(FontShader::defaultInstance().uniformLocs));
	t.m_impl->quads.reserve(textLen * 2);

	Vec3f pos = {};
	FontMap& fm = m_impl->glyphs[size];

	for (const char* c = text; *c; c++)
	{
		Quad q = Quad::create();

		Transform trans = Transform::create();
		const Glyph& g = fm.glyphs[*c];

		trans.setPosition({ pos.x + g.bitmap.x, pos.y + g.bitmap.y });
		q.translate(trans.modelMatrix());
		
		const Rectui r = {
			uint32_t(0),
			uint32_t(0),
			uint32_t(fm.width),
			uint32_t(fm.height)
		};
		q.setTextureCoords(r, { int(fm.width), int(fm.height) });

		//q.verts[0].texCoord.u = g.xOffsetCoord;
		//q.verts[0].texCoord.v = g.bitmap.h / fm.height;

		//q.verts[1].texCoord.u = g.xOffsetCoord;
		//q.verts[1].texCoord.v = 0.f;

		//q.verts[2].texCoord.u = g.xOffsetCoord + g.bitmap.w / fm.width;
		//q.verts[2].texCoord.v = 0.f;

		//q.verts[3].texCoord.u = g.xOffsetCoord + g.bitmap.w / fm.width;
		//q.verts[3].texCoord.v = g.bitmap.h / fm.height;

		pos += g.advance;

		t.m_impl->quads.push_back(q);
	}

	const auto* verts = reinterpret_cast<Vertex2D*>(&t.m_impl->quads[0]);
	const size_t numVerts = t.m_impl->quads.size() * Quad::VERT_COUNT;
	t.m_impl->mesh.vbo.writeBuffer(verts, numVerts, 0);

	return t;
}
