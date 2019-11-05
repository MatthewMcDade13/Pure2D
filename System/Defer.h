#ifndef PURE2D_SYSTEM_DEFER_H
#define PURE2D_SYSTEM_DEFER_H

#include <cstdio>
#include <utility>
#include <Pure2D/NonCopyable>;

namespace pure
{
	namespace internal
	{
		template <typename Func>
		struct Defer
		{
			Func f;
			bool isValid = true;

			Defer(const Defer&) = delete;
			constexpr Defer(Defer&& other) : f(std::move(other.f)) { isValid = false; }
			constexpr Defer(const Func&& f) : f(std::forward<const Func>(f)) {}

			~Defer() { if (isValid)	f(); }
		};

		constexpr struct
		{
			template<typename Func>
			constexpr Defer<Func> operator <<(const Func&& f) const
			{
				return Defer<Func>(std::forward<const Func>(f));
			}
		} defer_helper;
	}

	template<typename T> struct Scoped;

	template<typename T>
	Scoped<T> makeScoped(const T& item)
	{
		return Scoped<T>::make(item);
	}

	template<typename Resource>
	struct Scoped : private NonCopyable
	{
		static inline Scoped<Resource> make(const Resource& r)
		{
			return { r };
		}

		Resource& borrow() { return resource; }
		Resource* operator->() { return &resource; }

		~Scoped() 
		{
			resource.free();
		}

	private:
		Scoped(const Resource& resource): resource(r) { }

		Resource resource;
	};
}


#define DEFER_CONCAT(x, y) x##y
#define DEFER_CONCAT_HELPER(x, y) (DEFER_CONCAT(x, y))
#define DEFER_VAR_NAME DEFER_CONCAT_HELPER(s_defer_, __LINE__)

#define defer const auto DEFER_VAR_NAME = pure::internal::defer_helper << [&]

#undef DEFER_CONCAT
#undef DEFER_CONCAT_HELPER
#undef DEFER_VAR_NAME

#endif // PURE2D_SYSTEM_DEFER_H