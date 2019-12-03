#pragma once

#if defined(_WIN32) || defined(_WIN64)
	#if defined(_DEBUG)
	#define PURE_DEBUG 1
	#else
	#define PURE_DEBUG 0
	#endif

#elif defined (NDEBUG)
	#define PURE_DEBUG 0
#else
	#define PURE_DEBUG 1
#endif


