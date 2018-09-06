#include "renderer_stdafx.h"

#include "Components/MeshComponent.h"

using namespace Renderer;

MeshComponent::MeshComponent(COMPONENT_INIT_PARAMS, Resources::Mesh * p_model) :
	COMPONENT_INIT(MeshComponent), m_model(p_model)
{
	if (m_model) m_model->InitBuffers(m_va, m_layout);
}

void MeshComponent::SetModel(Resources::Mesh* p_model)
{
	if (p_model)
	{
		m_model = p_model;
		m_model->InitBuffers(m_va, m_layout);
	}
}

Resources::Mesh* MeshComponent::GetModel() const
{
	return m_model;
}

Resources::Buffers::VertexArray& MeshComponent::GetVertexArray()
{
	return m_va;
}

Resources::Buffers::VertexBufferLayout& MeshComponent::GetVertexLayout()
{
	return m_layout;
}