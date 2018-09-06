#pragma once

#include "renderer_stdafx.h"
#include "RendererAPI.h"

#include "Resources/Buffers/VertexArray.h"
#include "Resources/Mesh.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Renderer
{
	class RENDERER_API MeshComponent : public ECS::Component<MeshComponent>
	{
	private:
		Resources::Buffers::VertexArray m_va;
		Resources::Buffers::VertexBufferLayout m_layout;
		Resources::Mesh* m_model;

	public:
		MeshComponent(COMPONENT_INIT_PARAMS, Resources::Mesh* p_model = nullptr);

		inline void SetModel(Resources::Mesh* p_model);
		Resources::Mesh* GetModel() const;

		Resources::Buffers::VertexArray& GetVertexArray();
		Resources::Buffers::VertexBufferLayout& GetVertexLayout();
	};
}

#pragma warning(pop)