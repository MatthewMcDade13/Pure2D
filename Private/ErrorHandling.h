#pragma once


#include "Debug.h"

void checkAlError();
void checkGlError();

#if PURE_DEBUG

	#define alCall(caller) do { \
			caller; \
			checkAlError(); \
	} \
	while (false) \

	#define glCall(caller) do { \
			caller; \
			checkGlError(); \
	} \
	while (false) \

#else
	#undef alCall
	#undef glCall
	#define glCall
	#define alCall
#endif


