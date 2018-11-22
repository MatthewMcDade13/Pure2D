#ifndef PURE2D_TIME_H
#define PURE2D_TIME_H

#include <Pure2D/Define.h>

namespace pure
{
	namespace time
	{
		struct PURE2D_API Clock
		{
			float start;

			void init();
			float reset();
			float elapsedTime() const;
		};

		struct PURE2D_API Timer
		{
			float current;
			float duration;

			bool tick(float delta);
			void start(float duration);
			void reset();
		
		};


		PURE2D_API float getTime();
	}
}

#endif // PURE2D_TIME_H
