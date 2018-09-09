#ifndef PURE3D_ASSET_MANAGER_H
#define PURE3D_ASSET_MANAGER_H

#include <cinttypes>
#include "Define.h"
#include "System/NonCopyable.h"

namespace pure
{
	struct Shader;
	struct Texture;
	enum class TextureFormat;

	struct PURE2D_API AssetManager : private NonCopyable
	{
		AssetManager();
		~AssetManager();

		void freeShader(const char* shaderName);
		void freeTexture(const char* filePath);

		bool loadShader(const char* shaderName, const char* vertPath, const char* fragPath);
		bool loadShaderSrc(const char* shaderName, const char* vertSrc, const char* fragSrc);
		bool loadShader(const char* shaderName, Shader shader);
		bool loadTexture(const char* filePath, bool shouldFlip = true);


		Shader* getShader(const char* name);
		Texture* getTexture(const char* path);

	private:
		struct AssetManagerImpl* m_impl;
	};
}

#endif // PURE3D_ASSET_MANAGER_H

