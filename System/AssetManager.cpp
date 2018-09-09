#include "AssetManager.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Private/Util.h"

using namespace pure;


namespace pure
{
	struct AssetManagerImpl
	{
		std::unordered_map<std::string, Shader> shaders;
		std::unordered_map<std::string, Texture> textures;
	};
}

pure::AssetManager::AssetManager()
	: m_impl(new AssetManagerImpl()) { }

pure::AssetManager::~AssetManager()
{
	for (auto& kv : m_impl->shaders)
		kv.second.free();
	for (auto& kv : m_impl->textures)
		Texture::free(kv.second);

	delete m_impl;
}

bool pure::AssetManager::loadShader(const char * shaderName, const char * vertPath, const char * fragPath)
{
	Shader shader = Shader::create(vertPath, fragPath);
	if (shader.id_ == 0) return false;

	return loadShader(shaderName, shader);
}

bool pure::AssetManager::loadShaderSrc(const char * shaderName, const char * vertSrc, const char* fragSrc)
{
	Shader shader = Shader::createSrc(vertSrc, fragSrc);
	if (shader.id_ == 0) return false;

	return loadShader(shaderName, shader);
}

bool pure::AssetManager::loadShader(const char * shaderName, Shader shader)
{
	auto result = m_impl->shaders.emplace(shaderName, shader);
	return result.second;
}

bool pure::AssetManager::loadTexture(const char * filePath, bool shouldFlip)
{
	Texture tex = Texture::create(filePath, shouldFlip);
	if (tex.id_ == 0) return false;

	auto result = m_impl->textures.emplace(filePath, tex);
	return result.second;
}

Shader * pure::AssetManager::getShader(const char * name)
{
	return mapGet(m_impl->shaders, std::string(name));
}

Texture * pure::AssetManager::getTexture(const char * path)
{
	return mapGet(m_impl->textures, std::string(path));
}

void pure::AssetManager::freeShader(const char* shaderName)
{
	auto node = m_impl->shaders.extract(shaderName);
	if (node.empty()) return;

	node.mapped().free();
}

void pure::AssetManager::freeTexture(const char* filePath)
{
	auto node = m_impl->textures.extract(filePath);
	if (node.empty()) return;

	Texture::free(node.mapped());
}