#include "renderer_stdafx.h"

#include "Entities/TexturedMeshGameObject.h"

using namespace Renderer;

TexturedMeshGameObject::TexturedMeshGameObject(GAMEOBJECT_INIT_PARAMS, Resources::Mesh& p_mesh, Resources::Shader* p_shader, Resources::Texture* p_texture) : GAMEOBJECT_INIT(TexturedMeshGameObject)
{
	AddComponent<MeshComponent>(&p_mesh);
	AddComponent<MaterialComponent>(p_shader, p_texture);
}