#include "Shader.h"
//#include <gl/GL.h>
#include "glad.h"
#include "Private/FileIO.h"
#include "Private/Shaders.h"
#include "Math/Vec4.h"
#include "Math/Mat4.h"
#include "Private/Convert.h"
#include "System/Util.h"
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
static void fillCommonUniforms(Shader& s);

Shader pure::Shader::create(const char * vertShaderPath, const char * fragShaderPath)
{
	uint32_t vShader = createShaderFromFile(vertShaderPath, GL_VERTEX_SHADER);
	uint32_t fShader = createShaderFromFile(fragShaderPath, GL_FRAGMENT_SHADER);

	if (!vShader || !fShader) return { 0 };

	Shader s = { createShaderProg(vShader, fShader) };
	fillCommonUniforms(s);

	return s;
}

Shader pure::Shader::createSrc(const char * vertSrc, const char * fragSrc)
{
	uint32_t vShader = compileShader(vertSrc, GL_VERTEX_SHADER);
	uint32_t fShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

	if (!vShader || !fShader) return { 0 };

	Shader s = { createShaderProg(vShader, fShader) };
	fillCommonUniforms(s);

	return s;
}

void pure::Shader::bind() const
{
	glUseProgram(id_);
}

int pure::Shader::getLocation(const char *uniform) const
{
	return glGetUniformLocation(id_, uniform);
}

void pure::Shader::setUniform(int location, const Vec4<float> &vec) const
{
	bind();
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void pure::Shader::setUniform(int location, const Vec3<float> &vec) const
{
	bind();
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void pure::Shader::setUniform(int location, const Mat4 &matrix, bool transpose) const
{
	bind();
	const glm::mat4* mat = TO_GLM_MAT4_CONST(matrix.val_);
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(*mat));
}

void pure::Shader::setUniform(int location, float val) const
{
	bind();
	glUniform1f(location, val);
}

void pure::Shader::setUniform(int location, int val) const
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

static constexpr const char* FRAG_TEMPLATE_VARS = "#version 330\n"
       "out vec4 FragColor;\n"

       "in vec2 TexCoord;\n"
       "in vec4 Color;\n"
       "in vec3 FragPos;\n"

       "uniform sampler2D u_texture;\n";

// effect signature: vec4 effect(vec4 color, sampler2D tex, vec2 texCoords, vec3 fragPos)
static constexpr const char* FRAG_TEMPLATE_MAIN = "\n"
       "void main()\n"
       "{\n"
       "	FragColor = effect(Color, u_texture, TexCoord, FragPos);\n"
       "}";


static constexpr const char* VERT_TEMPLATE_DECLS = "#version 330\n"
	 "layout(location = 0) in vec3 l_pos;\n"
	 "layout(location = 1) in vec2 l_texCoords;\n"
	 "layout(location = 2) in vec4 l_color;\n"

	 "out vec2 TexCoord;\n"
	 "out vec4 Color;\n"
	 "out vec3 FragPos;\n"

	 "uniform mat4 u_matrixMVP;\n"
	 "uniform mat4 u_modelMatrix;\n"
     "mat4 matrixMVP = u_matrixMVP;\n";

static constexpr const char* VERT_TEMPLATE_DECLS_INSTANCED = "#version 330\n"
	  "layout(location = 0) in vec3 l_pos;\n"
	  "layout(location = 1) in vec2 l_texCoords;\n"
	  "layout(location = 2) in vec4 l_color;\n"
	  "layout(location = 3) in mat4 l_matrixMVP;\n"
	  "layout(location = 7) in mat4 u_modelMatrix;\n"

	  "out vec2 TexCoord;\n"
	  "out vec4 Color;\n"
	  "out vec3 FragPos;\n"
      "mat4 matrixMVP = l_matrixMVP;\n";

// position signature: vec4 position(mat4 mvpMatrix, vec3 fragPos)
static constexpr const char* VERT_TEMPLATE_MAIN = "\n"
		"void main()\n"
		"{\n"
		"	TexCoord = l_texCoords;\n"
		"	Color = l_color;\n"

		"   FragPos = vec3(u_modelMatrix * vec4(l_pos, 1.0));\n"

		"	gl_Position = position(matrixMVP, l_pos);\n"
		"}";


static const size_t fragTemplateLen = strlen(FRAG_TEMPLATE_VARS) + strlen(FRAG_TEMPLATE_MAIN);

// assume longer template just to keep things safe and concise
static const size_t vertTemplateLen = strlen(VERT_TEMPLATE_DECLS_INSTANCED) + strlen(VERT_TEMPLATE_MAIN);

static const size_t defaultFragLen = strlen(shader::frag);
static const size_t defaultInstancedVertLen = strlen(shader::instancedVert);
static const size_t defaultVertLen = strlen(shader::vert);

// TODO: Consider having this be the main way we create shaders?
// Having this as main way of creating shaders should suffice this engine is only doing 2D
void Shader::createFragShader(char* outBuffer, const char *effectSrc)
{
    std::string frag = FRAG_TEMPLATE_VARS;
    frag += effectSrc;
    frag += FRAG_TEMPLATE_MAIN;

    strcpy(outBuffer, frag.c_str());
}


void Shader::createVertShader(char *outBuffer, const char *positionSrc, bool isInstanced)
{
	std::string vert = isInstanced ? VERT_TEMPLATE_DECLS_INSTANCED : VERT_TEMPLATE_DECLS;
	vert += positionSrc;
	vert += VERT_TEMPLATE_MAIN;

	strcpy(outBuffer, vert.c_str());
}

Shader Shader::createDefault(bool isInstanced)
{
    return Shader::createSrc(isInstanced ? shader::instancedVert : shader::vert, shader::frag);
}

size_t Shader::getFragShaderSize(size_t inBufferCount)
{
	return fragTemplateLen + inBufferCount;
}

size_t Shader::getVertShaderSize(size_t inBufferCount)
{
	return vertTemplateLen + inBufferCount;
}

size_t Shader::getDefaultFragShaderSize() { return defaultFragLen; }
size_t Shader::getDefaultVertShaderSize(bool isInstanced) { return isInstanced ? defaultInstancedVertLen : defaultVertLen; }

void Shader::createDefaultFragShader(char *outBuffer)
{
	strcpy(outBuffer, shader::frag);
}

void Shader::createDefaultVertShader(char *outBuffer, bool isInstanced)
{
	strcpy(outBuffer, isInstanced ? shader::instancedVert : shader::vert);
}



// HELPER FUNCS
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

void fillCommonUniforms(Shader& s)
{
	s.mvpMatLoc_ = s.getLocation("u_matrixMVP");
	s.modelMatLoc_ = s.getLocation("u_modelMatrix");
}
