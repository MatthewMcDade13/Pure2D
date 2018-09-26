#include "Shader.h"
//#include <gl/GL.h>
#include "glad.h"
#include "Private/FileIO.h"
#include "Private/Shaders.h"
#include "Math/Vec4.h"
#include "Math/Mat4.h"
#include "Private/Convert.h"
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

using namespace pure;
using namespace pure::detail;

static uint32_t createShaderFromFile(const char* filePath, GLenum shaderType);
static uint32_t compileShader(const char* src, GLenum shaderType);
static uint32_t createShaderProg(uint32_t vert, uint32_t frag);
static bool checkShaderError(uint32_t shader, GLenum shaderQuery);
static bool checkProgramError(uint32_t program, GLenum query);

uint32_t createShaderFromFile(const char * filePath, GLenum shaderType)
{
	std::string shaderStr = readFile(filePath);
	const char* shaderCStr = shaderStr.c_str();
	return compileShader(shaderCStr, shaderType);
}

uint32_t compileShader(const char * src, GLenum shaderType)
{

	uint32_t shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	if (checkShaderError(shader, GL_COMPILE_STATUS))
	{
		std::cerr << "SHADER ERROR: \n" << src << std::endl;
		return 0;
	}

	return shader;
}

uint32_t createShaderProg(uint32_t vert, uint32_t frag)
{
	uint32_t shaderId = glCreateProgram();

	glAttachShader(shaderId, vert);
	glAttachShader(shaderId, frag);
	glLinkProgram(shaderId);

	if (checkProgramError(shaderId, GL_LINK_STATUS))
	{
		std::cerr << "SHADER LINK ERROR \n";
		glDeleteProgram(shaderId);
		shaderId = 0;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return shaderId;
}


bool checkShaderError(uint32_t shader, GLenum shaderQuery)
{
	char infoLog[512];
	int success;
	glGetShaderiv(shader, shaderQuery, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		return true;
	}

	return false;
}

bool checkProgramError(uint32_t program, GLenum query)
{
	char infoLog[512];
	int success;
	glGetProgramiv(program, query, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		return true;
	}

	return false;
}

Shader pure::Shader::create(const char * vertShaderPath, const char * fragShaderPath)
{
	uint32_t vShader = createShaderFromFile(vertShaderPath, GL_VERTEX_SHADER);
	uint32_t fShader = createShaderFromFile(fragShaderPath, GL_FRAGMENT_SHADER);

	if (!vShader || !fShader) return { 0 };

	return { createShaderProg(vShader, fShader) };
}

Shader pure::Shader::createSrc(const char * vertSrc, const char * fragSrc)
{
	uint32_t vShader = compileShader(vertSrc, GL_VERTEX_SHADER);
	uint32_t fShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

	if (!vShader || !fShader) return { 0 };

	return { createShaderProg(vShader, fShader) };
}

void pure::Shader::bind() const
{
	glUseProgram(id_);
}

uint32_t pure::Shader::getLocation(const char * uniform) const
{
	return glGetUniformLocation(id_, uniform);
}

void pure::Shader::setUniform(uint32_t location, const Vec4<float>& vec) const
{
	bind();
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void pure::Shader::setUniform(uint32_t location, const Vec3<float>& vec) const
{
	bind();
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void pure::Shader::setUniform(uint32_t location, const Mat4 & matrix, bool transpose) const
{
	bind();
	const glm::mat4* mat = TO_GLM_MAT4_CONST(matrix.val_);
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(*mat));
}

void pure::Shader::setUniform(uint32_t location, float val) const
{
	bind();
	glUniform1f(location, val);
}

void pure::Shader::setUniform(uint32_t location, int val) const
{
	bind();
	glUniform1i(location, val);
}

void pure::Shader::setUniform(const char * uniform, const Vec4<float>& vec) const
{
	setUniform(getLocation(uniform), vec);
}

void pure::Shader::setUniform(const char * uniform, const Vec3<float>& vec) const
{
	setUniform(getLocation(uniform), vec);
}

void pure::Shader::setUniform(const char * uniform, const Mat4 & matrix, bool transpose) const
{
	setUniform(getLocation(uniform), matrix, transpose);
}

void pure::Shader::setUniform(const char * uniform, float val) const
{
	setUniform(getLocation(uniform), val);
}

void pure::Shader::setUniform(const char * uniform, int val) const
{
	setUniform(getLocation(uniform), val);
}

void pure::Shader::free()
{
	glDeleteProgram(id_);
	id_ = 0;
}

constexpr const char* FRAG_TEMPLATE_VARS = "#version 330\n"
       "out vec4 FragColor;\n"

       "in vec2 TexCoord;\n"
       "in vec4 Color;\n"
       "in vec3 FragPos;\n"

       "uniform sampler2D u_texture;\n";

constexpr const char* FRAG_TEMPLATE_MAIN = "\n"
       "void main()\n"
       "{\n"
       "	FragColor = effect(Color, u_texture, TexCoord, FragPos);\n"
       "}";

// TODO: Consider having this be the main way we create shaders?
// Having this as main way of creating shaders should suffice this engine is only doing 2D
Shader Shader::createTemplated(const char *effectSrc, bool isInstanced)
{
    const char* vert = isInstanced ? shader::instancedVert : shader::vert;

    std::string frag = FRAG_TEMPLATE_VARS;
    frag += effectSrc;
    frag += FRAG_TEMPLATE_MAIN;

    return Shader::createSrc(vert, frag.c_str());

}

