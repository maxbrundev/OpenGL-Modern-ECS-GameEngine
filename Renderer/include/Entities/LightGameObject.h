#pragma once

#include "renderer_stdafx.h"
#include "RendererAPI.h"

namespace Renderer
{
	class RENDERER_API LightGameObject : public Core::GameObject<LightGameObject>
	{
	public:
		LightGameObject(GAMEOBJECT_INIT_PARAMS);
		~LightGameObject();
	};
}
