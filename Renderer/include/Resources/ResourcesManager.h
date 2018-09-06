#pragma once
#include "renderer_stdafx.h"

#include "RendererAPI.h"

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Renderer
{
	namespace Resources
	{
		using Model_ptr = std::unique_ptr<Mesh>;
		using Shader_ptr = std::unique_ptr<Shader>;
		using Texture_ptr = std::unique_ptr<Texture>;

		class RENDERER_API ResourcesManager
		{
		private:
			std::string m_meshRootDir;
			std::string m_shaderRootDir;
			std::string m_textureRootDir;

			std::unordered_map<std::string, Model_ptr>		m_meshResources;
			std::unordered_map<std::string, Shader_ptr>		m_shaderResources;
			std::unordered_map<std::string, Texture_ptr>	m_textureResources;

		public:
			ResourcesManager();
			ResourcesManager(const ResourcesManager& other) = delete;
			void operator=(const ResourcesManager& other) = delete;
			~ResourcesManager() = default;

			void SetMeshRootDir(const std::string&& p_directory) { m_meshRootDir = p_directory; }
			void SetShaderRootDir(const std::string&& p_directory) { m_shaderRootDir = p_directory; }
			void SetTextureRootDir(const std::string&& p_directory) { m_textureRootDir = p_directory; }

			Mesh& LoadMesh(const std::string&& p_name, const std::string&& p_fileName, bool p_forceReload = false);
			Shader& LoadShader(const std::string&& p_name, const std::string&& p_fileName, bool p_forceReload = false);
			Texture& LoadTexture(const std::string&& p_name, const std::string&& p_fileName, bool p_forceReload = false);

			Mesh& GetMesh(const std::string&& p_name) { return *m_meshResources.at(p_name); }
			Shader& GetShader(const std::string&& p_name) { return *m_shaderResources.at(p_name); }
			Texture& GetTexture(const std::string&& p_name) { return *m_textureResources.at(p_name); }
		};
	}
}

#pragma warning(pop)