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

		template<typename T>
		using EnableIfIntegral = typename std::enable_if_t<std::is_integral_v<T>>;

		template<typename T>
		using EnableIfFloating = typename std::enable_if_t<std::is_floating_point_v<T>>;

		template<typename T>
		using EnableIfClass = typename std::enable_if_t<std::is_class_v<T>>;


		template <typename T>
		EnableIfIntegral<T> print(T s) { dbout() << s; }

		template <typename T>
		EnableIfFloating<T> print(T s) { dbout() << s; }

		template <typename Printable>
		EnableIfClass<Printable> print(const Printable& obj)
		{ dbout() << (obj.toString()); }

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