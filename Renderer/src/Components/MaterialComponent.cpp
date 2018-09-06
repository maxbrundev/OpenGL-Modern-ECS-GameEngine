#include "renderer_stdafx.h"
#include <Components/MaterialComponent.h>

using namespace Renderer;

MaterialComponent::MaterialComponent(COMPONENT_INIT_PARAMS, Resources::Shader* p_shader, Resources::Texture* p_texture):
	COMPONENT_INIT(MaterialComponent), 
	m_ambient(0.5f, 0.5f, 0.5f),
	m_diffuse(1.0f, 1.0f, 1.0f),
	m_specular(0.5f, 0.5f, 0.5f), 
	m_shininess(32.f),
	m_shader(p_shader), m_texture(p_texture)
{
	if (m_texture && m_shader)
	{
		m_shader->SetUniform("u_texture", 0);
		m_texture->Bind();
	}
}

Resources::Shader* MaterialComponent::GetShader() const
{
	return m_shader;
}

Resources::Texture* MaterialComponent::GetTexture() const
{
	return m_texture;
}

const Core::Maths::vec3f& MaterialComponent::GetAmbient() const
{
	return m_ambient;
}

const Core::Maths::vec3f& MaterialComponent::GetDiffuse() const
{
	return m_diffuse;
}

const Core::Maths::vec3f& MaterialComponent::GetSpecular() const
{
	return m_specular;
}

float MaterialComponent::GetShininess() const
{
	return m_shininess;
}

void MaterialComponent::SetShader(Resources::Shader* p_shader)
{
	if (p_shader)
	{
		m_shader = p_shader;
	}
}

void MaterialComponent::SetTexture(Resources::Texture* p_texture)
{
	if (p_texture)
	{
		m_texture = p_texture;
	}
}
