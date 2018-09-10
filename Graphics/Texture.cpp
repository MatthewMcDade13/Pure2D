#include "Texture.h"
#include <iostream>
#include <cassert>
#include "glad.h"
#include "stb_image.h"

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
	glActiveTexture(m_unit);
	glBindTexture(GL_TEXTURE_2D, id_);
}

Texture pure::Texture::create(const char * fileName, bool shouldFlip)
{
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

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


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
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

void pure::Texture::free(Texture & tex)
{
	glDeleteTextures(1, &tex.id_);
	tex = { };
}
