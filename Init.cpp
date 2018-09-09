//
// Created by matt on 7/14/18.
//

#include "Init.h"
#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool pure::init()
{
	if (glfwInit())
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		return true;
	}

	return false;
}

void pure::terminate()
{
	glfwTerminate();
}
