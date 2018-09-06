#pragma once

#include "renderer_stdafx.h"

#include "RendererAPI.h"

#include "Components/MeshComponent.h"
#include "Components/MaterialComponent.h"

namespace Renderer
{
	class RENDERER_API TexturedMeshGameObject : public Core::GameObject<TexturedMeshGameObject>
	{
	public:
		TexturedMeshGameObject(GAMEOBJECT_INIT_PARAMS, Resources::Mesh& p_mesh, Resources::Shader* p_shader, Resources::Texture* p_texture);
		~TexturedMeshGameObject() = default;
	};
}