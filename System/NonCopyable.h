#ifndef PURE2D_NONCOPYABLE_H
#define PURE2D_NONCOPYABLE_H

#include <Pure2D/Define.h>

namespace pure
{
	class PURE2D_API NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
	private:
		NonCopyable(const NonCopyable&);
		NonCopyable& operator=(const NonCopyable&);
	};
}


#endif // PURE2D_NONCOPYABLE_H
