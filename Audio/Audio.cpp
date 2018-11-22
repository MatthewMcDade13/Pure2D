#include "Audio.h"
#include <iostream>
#include <Pure2D/Audio/Listener.h>
#include <SDL2/SDL_mixer.h>
#include <AL/al.h>
#include <AL/alc.h>
#include "Private/ErrorHandling.h"

static constexpr int frequency = 48000;
static constexpr int chunkSize = 4096;

static ALCdevice* device = nullptr;
static ALCcontext* context = nullptr;

bool pure::initAudio()
{
	// NOTE/BOTTLENECK: We are only using SDL_mixer for loading audio files easily. Sadly it looks like
	// its API requires it to open an Audio Device on its own. Not sure what the performance
	// implication of this are, but if there are issues you can probably look here.
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
		Mix_Quit();
		return false;
	}

	device = alcOpenDevice(nullptr);
	if (!device)
	{
		std::cerr << "OpenAL :: Could not load Audio Device: " << alGetError() << std::endl;
		Mix_Quit();
		return false;
	}

	context = alcCreateContext(device, nullptr);

	if (!alcMakeContextCurrent(context))
	{
		std::cerr << "OpenAL :: Could not make context current: " << alGetError() << std::endl;
		Mix_Quit();
		alcCloseDevice(device);
		return false;
	}

	listener::resetDefaults();

	return true;
}

void pure::terminateAudio()
{
	Mix_Quit();
	alCall(alcDestroyContext(context));
	alCall(alcCloseDevice(device));
}
