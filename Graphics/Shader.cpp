#include "Shader.h"
//#include <gl/GL.h>
#include "External/glad.h"
#include "Private/FileIO.h"
#include "Private/GlContext.h"
#include "Private/Shaders.h"
#include "Math/Vec4.h"
#include "Math/Mat4.h"
#include "Private/Convert.h"
#include "Private/Util.h"
#include "System/Util.h"
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

using namespace pure;

static uint32_t createShaderFromFile(const char* filePath, GLenum shaderType);
static std::string createVertShader(const char *positionSrc, bool isInstanced);
static std::string createFragShader(const char *effectSrc);
static uint32_t compileShader(const char* src, GLenum shaderType);
static uint32_t createShaderProg(uint32_t vert, uint32_t frag);
static bool checkShaderError(uint32_t shader, GLenum shaderQuery);
static bool checkProgramError(uint32_t program, GLenum query);
static void fillCommonUniforms(Shader& s);

pure::Shader::Shader(uint32_t id) :
	m_id(id), 
	locations({ })
{
	locations.resize(DEFAULT_LOC_COUNT);
}

int pure::Shader::storeLocation(const char* locationName)
{
	int loc = getLocation(locationName);
	locations.push_back(loc);
	return loc;
}

Shader pure::Shader::make(bool isInstanced)
{
	return Shader::fromSrc(isInstanced ? shader::instancedVert : shader::vert, shader::frag);
}

Shader pure::Shader::fromFile(const char * vertShaderPath, const char * fragShaderPath)
{
	uint32_t vShader = createShaderFromFile(vertShaderPath, GL_VERTEX_SHADER);
	uint32_t fShader = createShaderFromFile(fragShaderPath, GL_FRAGMENT_SHADER);

	if (!vShader || !fShader) return { 0 };

	Shader s = { createShaderProg(vShader, fShader) };
	fillCommonUniforms(s);

	return s;
}

Shader pure::Shader::fromSrc(const char * vertSrc, const char * fragSrc)
{
	uint32_t vShader = compileShader(vertSrc, GL_VERTEX_SHADER);
	uint32_t fShader = compileShader(fragSrc, GL_FRAGMENT_SHADER);

	if (!vShader || !fShader) return { 0 };

	Shader s = { createShaderProg(vShader, fShader) };
	fillCommonUniforms(s);

	return s;
}

Shader pure::Shader::fromTemplate(const char * positionSrc, const char * effectSrc, bool isInstanced)
{
	std::string vert = "";
	if (positionSrc == nullptr)
		vert = isInstanced ? shader::instancedVert : shader::vert;
	else
		vert = createVertShader(positionSrc, isInstanced);

	std::string frag = effectSrc == nullptr ? shader::frag : createFragShader(effectSrc);

	return fromSrc(vert.c_str(), frag.c_str());
}

void pure::Shader::bind() const
{
	if (gl::isStateBound(gl::BindState::SHADER_PROGRAM, m_id)) return;
	gl::bindShader(m_id);
}

int pure::Shader::getLocation(const char *uniform) const
{
	return glGetUniformLocation(m_id, uniform);
}

template<typename T>
static void setUniformIndx_(const Shader& s, int index, T val)
{
	vec_bounds_assert(s.locations, index);
	int loc = s.locations[index];
	s.setUniform(loc, val);
}

void pure::Shader::setUniformIndx(int index, const Vec4<float>& vec) const
{
	setUniformIndx_<const Vec4<float>&>(*this, index, vec);
}

void pure::Shader::setUniformIndx(int index, const Vec3<float>& vec) const
{
	setUniformIndx_<const Vec3<float>&>(*this, index, vec);
}

void pure::Shader::setUniformIndx(int index, Vec2f vec) const
{
	setUniformIndx_<const Vec2f>(*this, index, vec);
}

void pure::Shader::setUniformIndx(int index, const Mat4 & matrix, bool transpose) const
{
	vec_bounds_assert(locations, index);
	int loc = locations[index];
	setUniform(loc, matrix, transpose);
}

void pure::Shader::setUniformIndx(int index, float val) const
{
	setUniformIndx_(*this, index, val);
}

void pure::Shader::setUniformIndx(int index, int val) const
{
	setUniformIndx_(*this, index, val);
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

void pure::Shader::setUniform(int location, Vec2f vec) const
{
	bind();
	glUniform2f(location, vec.x, vec.y);
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

void pure::Shader::setUniform(const char* uniform, Vec2f vec) const
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
	locations.clear();
	glDeleteProgram(m_id);
	m_id = 0;
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

std::string createFragShader(const char *effectSrc)
{
	std::string frag = FRAG_TEMPLATE_VARS;
	frag += effectSrc;
	frag += FRAG_TEMPLATE_MAIN;

	return frag;
}

std::string createVertShader(const char *positionSrc, bool isInstanced)
{
	std::string vert = isInstanced ? VERT_TEMPLATE_DECLS_INSTANCED : VERT_TEMPLATE_DECLS;
	vert += positionSrc;
	vert += VERT_TEMPLATE_MAIN;

	return vert;
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
	s.locations[Shader::MVP_MAT_LOC] = s.getLocation("u_matrixMVP");
	s.locations[Shader::MODEL_MAT_LOC] = s.getLocation("u_modelMatrix");
}
