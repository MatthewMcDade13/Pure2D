#ifndef PURE2D_PRIVATE_SHADERS_H
#define PURE2D_PRIVATE_SHADERS_H

namespace shader
{
	constexpr const char* vert = "#version 330\n"
		"layout(location = 0) in vec3 l_pos;\n"
		"layout(location = 1) in vec2 l_texCoords;\n"
		"layout(location = 2) in vec4 l_color;\n"

		"out vec2 TexCoord;\n"
		"out vec4 Color;\n"
        "out vec3 FragPos;\n"

		"uniform sampler2D u_texture;\n"
		"uniform mat4 u_matrixMVP;\n"
        "uniform mat4 u_modelMatrix;\n"
		"uniform vec4 u_textureOffsets;\n" // position = xy, size = zw

		"void main()\n"
		"{\n"
		"   vec2 texSize = textureSize(u_texture, 0);\n"
		"   vec2 texDim = u_textureOffsets.zw * l_texCoords;\n"
		"   vec2 texCoord = texDim + u_textureOffsets.xy;\n"
		"   vec2 normalizedCoords = vec2(texCoord.x / texSize.x, 1.0 - (texCoord.y / texSize.y));\n"

		"	TexCoord = normalizedCoords;\n"
		"   Color = l_color;\n"
        "   FragPos = vec3(u_modelMatrix * vec4(l_pos, 1.0));\n"

		"	gl_Position = u_matrixMVP * vec4(l_pos, 1.0);\n"
		"}";

	constexpr const char* primVert = "#version 330\n"
		"layout(location = 0) in vec3 l_pos;\n"
		"layout(location = 1) in vec2 l_texCoords;\n"
		"layout(location = 2) in vec4 l_color;\n"

		"out vec2 TexCoord;\n"
		"out vec4 Color;\n"
		"out vec3 FragPos;\n"

		"uniform mat4 u_matrixMVP;\n"
		"uniform mat4 u_modelMatrix;\n"

		"void main()\n"
		"{\n"
		"	TexCoord = l_texCoords;\n"
		"   Color = l_color;\n"
		"   FragPos = vec3(u_modelMatrix * vec4(l_pos, 1.0));\n"

		"	gl_Position = u_matrixMVP * vec4(l_pos, 1.0);\n"
		"}";

	constexpr const char* frag = "#version 330\n"
         "out vec4 FragColor;\n"

         "in vec4 Color;\n"

         "void main()\n"
         "{\n"
         "	FragColor = Color;"
         "}";

	constexpr const char* instancedSpriteVert = "#version 330\n"
		"layout(location = 0) in vec3 l_pos;\n"
		"layout(location = 1) in vec2 l_texCoords;\n"
		"layout(location = 2) in vec4 l_color;\n"
		"layout(location = 3) in vec4 l_textureOffsets;\n"
		"layout(location = 4) in mat4 l_matrixMVP;\n"
        "layout(location = 8) in mat4 u_modelMatrix;\n"

		"out vec2 TexCoord;\n"
        "out vec4 Color;\n"
        "out vec3 FragPos;\n"
		"uniform sampler2D u_texture;\n"

		"void main()\n"
		"{\n"
		"   vec2 texSize = textureSize(u_texture, 0);\n"
		"   vec2 texDim = l_textureOffsets.zw * l_texCoords;\n"
		"   vec2 texCoord = texDim + l_textureOffsets.xy;\n"
		"   vec2 normalizedCoords = vec2(texCoord.x / texSize.x, 1.0 - (texCoord.y / texSize.y));\n"

		"	TexCoord = normalizedCoords;\n"
        "	Color = l_color;\n"

        "   FragPos = vec3(u_modelMatrix * vec4(l_pos, 1.0));\n"

		"	gl_Position = l_matrixMVP * vec4(l_pos, 1.0);\n"
		"}";


	constexpr const char* spriteFrag = "#version 330\n"
		"out vec4 FragColor;\n"

		"in vec2 TexCoord;\n"

		"uniform sampler2D u_texture;\n"

		"void main()\n"
		"{\n"
		"	FragColor = texture(u_texture, TexCoord);\n"
		"}";


	constexpr const char* colorFrag = "#version 330\n"
		"out vec4 FragColor;\n"

		"in vec4 Color;\n"
		"uniform vec4 u_color;\n"

		"void main()\n"
		"{\n"
		"	FragColor = Color * u_color;"
		"}";

}

#endif // PURE2D_PRIVATE_SHADERS_H