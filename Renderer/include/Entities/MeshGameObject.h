#pragma once

#include "renderer_stdafx.h"
#include "RendererAPI.h"

namespace Renderer
{
	class RENDERER_API MeshGameObject : public Core::GameObject<MeshGameObject>
	{
	public:
		MeshGameObject(GAMEOBJECT_INIT_PARAMS);
		~MeshGameObject() = default;
	};
}