#include "SoundBuffer.h"

#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <AL/al.h>
#include "Private/ErrorHandling.h"

using namespace pure;

static constexpr ALenum toAlFormat(SoundFormat fmt)
{
	switch (fmt)
	{
		case SoundFormat::MONO8: return AL_FORMAT_MONO8;
		case SoundFormat::MONO16: return AL_FORMAT_MONO16;
		case SoundFormat::STEREO8: return AL_FORMAT_STEREO8;
		case SoundFormat::STEREO16: return AL_FORMAT_STEREO16;
		default: return 0;
	}
}

SoundBuffer pure::SoundBuffer::make()
{
	SoundBuffer sb = {};

	alCall(alGenBuffers(1, &sb.m_id));

	return sb;
}

uint32_t pure::SoundBuffer::id() const { return m_id; }

void pure::SoundBuffer::free()
{
	alCall(alDeleteBuffers(1, &m_id));
	m_id = 0;
}

void pure::SoundBuffer::write(SoundFormat format, void* data, size_t size, size_t frequency) const
{
	alCall(alBufferData(m_id, toAlFormat(format), data, size, frequency));
}

bool pure::SoundBuffer::loadFromFile(const char * filename)
{
	Mix_Chunk* chunk = Mix_LoadWAV(filename);
	if (!chunk)
	{
		std::cerr << "SDL_Mixer :: Failed to open audio file: " << filename << std::endl;
		std::cerr << "SDL_Mixer :: " << Mix_GetError() << std::endl;
		return false;
	}

	write(SoundFormat::STEREO16, chunk->abuf, chunk->alen);
	Mix_FreeChunk(chunk);

	return true;
}
