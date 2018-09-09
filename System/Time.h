#ifndef PURE3D_TIME_H
#define PURE3D_TIME_H

#include "Define.h"

namespace pure
{
	namespace time
	{
		struct Clock
		{
			float start;
		};


		PURE2D_API float getTime();
		PURE2D_API void initClock(Clock& c);
		PURE2D_API float resetClock(Clock& c);
		PURE2D_API float getElapsedTime(Clock c);
	}
}

#endif // PURE3D_TIME_H
