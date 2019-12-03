#include "ErrorHandling.h"

#include "External/glad.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

void checkAlError()
{
	ALCenum error = alGetError(); 
		if (error != AL_NO_ERROR) 
			std::cerr << "OpenAL :: Errno: " << error << std::endl; 
}

void checkGlError()
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) 
	{ 
		std::cerr << "OpenGL :: Errno: " << error << std::endl; 
	}
}
