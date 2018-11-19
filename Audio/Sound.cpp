#include "Sound.h"
#include <SDL2/SDL_mixer.h>
#include <AL/al.h>
#include <iostream>

#define sdlChunk(handle) (static_cast<Mix_Chunk*>(handle))

using namespace pure;
using namespace audio;


void pure::audio::Sound::free()
{
	Mix_FreeChunk(sdlChunk(m_handle));
}

bool pure::audio::Sound::loadWavFromFile(const char * filename)
{
	m_handle = Mix_LoadWAV(filename);
	if (!m_handle)
	{
		std::cout << "SDL_Mixer :: Could not load wav file: " << filename << std::endl;
		std::cout << "SDL_Mixer :: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void Sound::setVolume(float percentage)
{
	const int volume = static_cast<int>(float(MIX_MAX_VOLUME) * percentage);
	Mix_VolumeChunk(sdlChunk(m_handle), volume);
}

bool pure::audio::Sound::play(int loops, int channel)
{
	Mix_Chunk* c = sdlChunk(m_handle);

	return Mix_PlayChannel(channel, sdlChunk(m_handle), loops) != -1;
}
