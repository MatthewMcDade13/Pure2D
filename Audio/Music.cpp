#include "Music.h"
#include <SDL2/SDL_mixer.h>
#include <AL/al.h>
#include <iostream>

#define sdlMusic(handle) (static_cast<Mix_Music*>(handle))

// TODO: Eventually we want to implement a way to stream sound through OpenAL instead of
// relying on SDL_Mixer so that we can have a more appropriate 3D music.

using namespace pure;

void pure::Music::free()
{
	Mix_FreeMusic(sdlMusic(m_handle));
}

bool pure::Music::loadFromFile(const char * filename)
{
	m_handle = Mix_LoadMUS(filename);
	if (!m_handle)
	{
		std::cout << "SDL_Mixer :: Could not load wav file: " << filename << std::endl;
		std::cout << "SDL_Mixer :: " << Mix_GetError() << std::endl;
		return false;
	}
	return true;
}

void Music::setVolume(float percentage)
{
	const int volume = static_cast<int>(float(MIX_MAX_VOLUME) * (percentage * 0.01f));
	Mix_VolumeMusic(volume);
}

bool pure::Music::play(int loops)
{
	return Mix_PlayMusic(sdlMusic(m_handle), loops);
}
