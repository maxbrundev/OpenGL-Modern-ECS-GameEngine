#include "renderer_stdafx.h"

#include <Resources/ResourcesManager.h>

using namespace Renderer;
using namespace Resources;

ResourcesManager::ResourcesManager() : m_meshRootDir("ressource/models/"), 
									   m_shaderRootDir("ressource/shaders/"),
									   m_textureRootDir("ressource/textures/")
{}

Mesh& ResourcesManager::LoadMesh(const std::string&& p_name, const std::string&& p_fileName, const bool p_forceReload)
{
	if (!p_forceReload && m_meshResources.find(p_name) != m_meshResources.end())
		return *m_meshResources[p_name].get();

	const auto res = m_meshResources.emplace(p_name, std::make_unique<Mesh>(m_meshRootDir + p_fileName));
	return *res.first->second.get();
}

Shader& ResourcesManager::LoadShader(const std::string&& p_name, const std::string&& p_fileName, const bool p_forceReload)
{
	if (!p_forceReload && m_shaderResources.find(p_name) != m_shaderResources.end())
		return *m_shaderResources[p_name].get();

	const auto res = m_shaderResources.emplace(p_name, std::make_unique<Shader>(m_shaderRootDir + p_fileName));
	return *res.first->second.get();
}

Texture& ResourcesManager::LoadTexture(const std::string&& p_name, const std::string&& p_fileName, const bool p_forceReload)
{
	if (!p_forceReload && m_textureResources.find(p_name) != m_textureResources.end())
		return *m_textureResources[p_name].get();

	const auto res = m_textureResources.emplace(p_name, std::make_unique<Texture>(m_textureRootDir + p_fileName));
	return *res.first->second.get();
}