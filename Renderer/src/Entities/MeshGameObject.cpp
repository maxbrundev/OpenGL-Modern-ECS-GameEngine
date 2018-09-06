#include "renderer_stdafx.h"

#include "Entities/MeshGameObject.h"
#include "Components/MeshComponent.h"

using namespace Renderer;

MeshGameObject::MeshGameObject(GAMEOBJECT_INIT_PARAMS) : GAMEOBJECT_INIT(MeshGameObject)
{
	AddComponent<MeshComponent>();
}