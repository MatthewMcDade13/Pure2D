#include "Time.h"
#include <GLFW/glfw3.h>

using namespace pure;
using namespace pure::time;

float pure::time::getTime()
{
	return static_cast<float>(glfwGetTime());
}

void pure::time::Clock::init()
{
	start = static_cast<float>(glfwGetTime());
}

float pure::time::Clock::reset()
{
	const float elapsed = elapsedTime();
	start = getTime();
	return elapsed;
}

float pure::time::Clock::elapsedTime() const
{
	return static_cast<float>(getTime() - start);
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
