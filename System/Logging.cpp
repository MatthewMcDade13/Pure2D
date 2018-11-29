#ifndef PURE2D_SYSTEM_LOGGING_H
#define PURE2D_SYSTEM_LOGGING_H

#include <Pure2D/Define.h>
#include <iostream>
#include <type_traits>

namespace pure
{
	namespace debug
	{
		std::ostream& dbout()
		{
			static std::ostream stream(std::cout.rdbuf());
			return stream;
		}

		// prints given arg to stdout
		template <typename T>
		typename std::enable_if<std::is_integral<T>::value>::type print(const T& s) 
		{ dbout() << s; }

		template <typename T>
		typename std::enable_if<std::is_floating_point<T>::value>::type print(const T& s) 
		{ dbout() << s; }

		inline void print(const Printable& obj) { dbout() << (obj.toString()); }

		inline void print(const char* s) { dbout() << s; }

		// sends newline and flushes stdout
		inline void cLog() { dbout() << std::endl; }

		// prints all args to stdout and then flushes it
		template <typename T, typename... Args>
		void cLog(const T& s, const Args&... args)
		{
			print(s);
			cLog(args...);
		}
	}
}

#if PURE_DEBUG
#define dbLog(...) pure::debug::cLog(__VA_ARGS__)
#else
#define dbLog(...)
#endif

#endif // PURE2D_SYSTEM_LOGGING_H