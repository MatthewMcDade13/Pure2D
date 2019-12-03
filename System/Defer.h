#ifndef PURE2D_SYSTEM_DEFER_H
#define PURE2D_SYSTEM_DEFER_H

#include <cstdio>
#include <utility>
#include <functional>
#include <Pure2D/System/NonCopyable.h>

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
}

#define DEFER_CONCAT(x, y) x##y
#define DEFER_CONCAT_HELPER(x, y) (DEFER_CONCAT(x, y))
#define DEFER_VAR_NAME DEFER_CONCAT_HELPER(s_defer_, __LINE__)

#define defer const auto DEFER_VAR_NAME = pure::internal::defer_helper << [&]

#undef DEFER_CONCAT
#undef DEFER_CONCAT_HELPER
#undef DEFER_VAR_NAME

namespace pure
{
	template<typename Resource>
	struct Scoped : private NonCopyable
	{
		using Deleter = std::function<void(Resource&)>;

		static const Deleter glResourceDeleter;

		static inline Scoped<Resource> make(const Resource& r, Deleter deleter = glResourceDeleter)
		{
			return { r, deleter };
		}

		Resource& borrow() { return resource; }

		Resource* operator->() { return &resource; }

		~Scoped()
		{
			deleter(resource);
		}
	private:
		constexpr Scoped(const Resource& r, Deleter deleter) : resource(r), deleter(deleter) { }
		Scoped(const Scoped<Resource>&&);
		Scoped<Resource>& operator=(const Scoped<Resource>&&);

		Resource resource;
		Deleter deleter;
	};

	template<typename T>
	const typename Scoped<T>::Deleter Scoped<T>::glResourceDeleter = [](T& r) { r.free(); };

	template<typename T>
	Scoped<T> makeScoped(const T& item, typename Scoped<T>::Deleter deleter = Scoped<T>::glResourceDeleter)
	{
		return Scoped<T>::make(item, deleter);
	}

}

#endif // PURE2D_SYSTEM_DEFER_H
