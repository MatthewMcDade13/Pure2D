//
// Created by matt on 7/14/18.
//

#include "Init.h"
#include "glad.h"
#include <SDL2/SDL.h>
#include <iostream>

bool pure::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL :: Failed to initialize" << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	return true;

}

void pure::terminate()
{
	SDL_Quit();
}
