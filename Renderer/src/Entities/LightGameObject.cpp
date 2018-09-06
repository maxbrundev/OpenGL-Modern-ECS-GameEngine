#include "renderer_stdafx.h"

#include <Entities/LightGameObject.h>

using namespace Renderer;

LightGameObject::LightGameObject(GAMEOBJECT_INIT_PARAMS) : GAMEOBJECT_INIT(LightGameObject)
{
	AddComponent<Core::TransformComponent>();
}

LightGameObject::~LightGameObject()
{
}