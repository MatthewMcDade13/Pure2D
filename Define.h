//
// Created by matt on 9/3/18.
//

#ifndef PURE2D_DEFINE_H
#define PURE2D_DEFINE_H

#if (defined(_WIN32) || defined(_WIN64)) && !defined(PURE2D_COMPILE_STATIC)
#if defined(PURE2D_EXPORTS)
#define PURE2D_API __declspec(dllexport)
#else
#define PURE2D_API __declspec(dllimport)
#endif
#else
#define PURE2D_API
#endif

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







#endif //PURE2D_DEFINE_H
