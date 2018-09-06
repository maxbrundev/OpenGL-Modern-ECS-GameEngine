#include "renderer_stdafx.h"

#include <Entities/CameraGameObject.h>
#include "Context/ContextEvents.h"

using namespace Renderer;
using namespace Core;

using namespace Renderer;

CameraGameObject::CameraGameObject(GAMEOBJECT_INIT_PARAMS) : 
	GAMEOBJECT_INIT(CameraGameObject),
	m_projection(glm::perspective(45.f, 1280 / 720.f, 0.1f, 100.0f))
{
	AddComponent<TransformComponent>();
	RegisterEventCallback(&CameraGameObject::OnFrameResize);
}

CameraGameObject::~CameraGameObject()
{
	UnregisterAllEventCallbacks();
}

void CameraGameObject::UseOrtho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
	m_projection = glm::ortho(left,right,bottom,top,zNear,zFar);
}

void CameraGameObject::UsePerspective(const float fov, const float aspect, const float zNear, const float zFar)
{
	m_projection = glm::perspective(fov, aspect, zNear, zFar);
}

glm::mat4 CameraGameObject::GetView() const
{
	return glm::inverse(GetComponent<TransformComponent>()->AsTransform().GetTransform());
}

const glm::mat4& CameraGameObject::GetProjection() const
{
	return m_projection;
}

void CameraGameObject::OnFrameResize(const Context::FrameBufferResizeEvent* e)
{
	m_projection = glm::perspective(45.f, e->width / static_cast<float>(e->height), 1.0f, 20.0f);
}

