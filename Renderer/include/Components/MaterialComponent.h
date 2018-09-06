#pragma once

#include "RendererAPI.h"

#include "ECS/Component/Component.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Renderer
{
	class RENDERER_API MaterialComponent : public ECS::Component<MaterialComponent>
	{
		Core::Maths::vec3f m_ambient;
		Core::Maths::vec3f m_diffuse;
		Core::Maths::vec3f m_specular;
		float m_shininess;
		Resources::Shader* m_shader;
		Resources::Texture* m_texture;
	public:
		MaterialComponent(COMPONENT_INIT_PARAMS, Resources::Shader* p_shader = nullptr, Resources::Texture* p_texture = nullptr);
		~MaterialComponent() = default;

		Resources::Shader* GetShader() const;
		Resources::Texture* GetTexture() const;
		const Core::Maths::vec3f& GetAmbient() const;
		const Core::Maths::vec3f& GetDiffuse() const;
		const Core::Maths::vec3f& GetSpecular() const;
		float GetShininess() const;

		inline void SetShader(Resources::Shader* p_shader);
		inline void SetTexture(Resources::Texture* p_texture);
	};
}

#pragma warning(pop)