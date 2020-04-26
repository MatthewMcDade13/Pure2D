#include "Texture.h"
#include <iostream>
#include <cassert>
#include "External/glad.h"
#include "Private/Convert.h"
#include "Private/GlContext.h"
#include "External/stb_image.h"

using namespace pure;


uint32_t pure::Texture::unit() const
{
	return m_unit;
}

void pure::Texture::setUnit(uint32_t unitNum)
{
	m_unit = GL_TEXTURE0 + unitNum;
}

void pure::Texture::bind() const
{
	if (gl::isStateBound(gl::BindState::TEXTURE, id_)) return;
	glActiveTexture(m_unit);
	gl::bindTexture(id_);
}

void pure::Texture::setAlignment(int alignment)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}

Texture pure::Texture::make(const char * fileName, bool shouldFlip)
{
	uint32_t texture;
	glGenTextures(1, &texture);
	gl::bindTexture(texture);

	// TODO: Create a function that can alter these values and do this there...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(shouldFlip);
	int width, height, nChannels;
	uint8_t* textureData = stbi_load(fileName, &width, &height, &nChannels, 0);
	if (textureData)
	{
		GLenum format = 0;
		switch (nChannels)
		{
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}


		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
			0, format, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load texture: " << fileName << std::endl;
		return {};
	}
	stbi_image_free(textureData);

	Texture t = {};
	t.size = { width, height };
	t.m_unit = GL_TEXTURE0;
	t.id_ = texture;

	return t;
}

Texture pure::Texture::make(size_t w, size_t h, Format internalFormat, Format format, void * data, DataType type)
{
	uint32_t texture;
	glGenTextures(1, &texture);
	gl::bindTexture(texture);

	glTexImage2D(GL_TEXTURE_2D, 0, toGlTexFormat(internalFormat), w, h,
		0, toGlTexFormat(format), toGlDataType(type), data);
	glGenerateMipmap(GL_TEXTURE_2D);

	Texture t = {};
	t.size = { int(w), int(h) };
	t.m_unit = GL_TEXTURE0;
	t.id_ = texture;
	return t;
}

void pure::Texture::free()
{
	glDeleteTextures(1, &id_);
	*this = { };
}

void pure::Texture::write(Vec2i offset, size_t w, size_t h, Format format, DataType type, void * data)
{
	bind();
	glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, w, h, 
		toGlTexFormat(format), toGlDataType(type), data);

}

// TODO: Verify this still works
Texture Texture::createBlank()
{
	GLubyte texdata[] = { 255, 255, 255, 255 };
	return Texture::make(1, 1, Format::RGBA, Format::RGBA, texdata);
}
