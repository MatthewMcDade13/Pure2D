#include "Sound.h"
#include <SDL2/SDL_mixer.h>

#include <iostream>

#define sdlChunk(handle) (static_cast<Mix_Chunk*>(handle))

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

bool pure::audio::Sound::play(int loops, int channel)
{
	return Mix_PlayChannel(channel, sdlChunk(m_handle), loops) != -1;
}
