#include "Time.h"
#include <GLFW/glfw3.h>

using namespace pure;
using namespace pure::time;

float pure::time::getTime()
{
	return static_cast<float>(glfwGetTime());
}

void pure::time::initClock(Clock & c)
{
	c.start = static_cast<float>(glfwGetTime());
}

float pure::time::resetClock(Clock & c)
{
	const float elapsed = getElapsedTime(c);
	c.start = static_cast<float>(glfwGetTime());
	return elapsed;
}

float pure::time::getElapsedTime(Clock c)
{
	return static_cast<float>(glfwGetTime() - c.start);
}
