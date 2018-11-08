//
// Created by matt on 7/14/18.
//

#include "Init.h"
#include "Private/FreeType.h"
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
	}
	else
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	if (!FreeType::get().init())
		return false;

	return true;
}

void pure::terminate()
{
	FreeType::get().destroy();
	glfwTerminate();
}
