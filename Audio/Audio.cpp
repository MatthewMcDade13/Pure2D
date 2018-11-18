#include "Audio.h"
#include <iostream>
#include <SDL2/SDL_mixer.h>

static constexpr int frequency = 22050;
static constexpr int chunkSize = 4096;

bool pure::audio::init()
{
	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, chunkSize) == -1)
	{
		std::cout << "SDL_Mixer :: Could not Open Audio" << std::endl;
		return false;
	}

	constexpr int flags = MIX_INIT_FLAC | MIX_INIT_MOD 
		| MIX_INIT_MP3 | MIX_INIT_OGG;
	int result = 0;

	if (flags != (result = Mix_Init(flags)))
	{
		std::cout << "SDL_Mixer :: Could not initialize mixer. Result: " << result << std::endl;
		std::cout << "SDL_Mixer :: " << Mix_GetError() << std::endl;
		return false;
	}
}

void pure::audio::terminate()
{
	Mix_Quit();
}
