#include "Time.h"
#include <SDL2/SDL.h>

using namespace pure;
using namespace pure::time;

float pure::time::getTime()
{
	return static_cast<float>(SDL_GetTicks());
}

void pure::time::Clock::init()
{
	start = getTime();
}

float pure::time::Clock::reset()
{
	const float elapsed = elapsedTime();
	start = getTime();
	return elapsed;
}

float pure::time::Clock::elapsedTime() const
{
	return static_cast<float>(getTime() - start) * .001f;
}

bool pure::time::Timer::tick(float delta)
{
	current += delta;
	if (current >= duration)
	{
		reset();
		return true;
	}

	return false;
}

void pure::time::Timer::start(float duration)
{
	current = 0.f;
	this->duration = duration;
}

void pure::time::Timer::reset()
{
	current = 0.f;
}
