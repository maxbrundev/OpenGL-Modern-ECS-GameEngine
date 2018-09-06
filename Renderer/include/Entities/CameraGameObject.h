#pragma once

#include "renderer_stdafx.h"
#include "RendererAPI.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Renderer
{
	namespace Context 
	{
		struct FrameBufferResizeEvent;
	}

	class RENDERER_API CameraGameObject : public Core::GameObject<CameraGameObject>, ECS::Event::IEventListener
	{
	private:
		glm::mat4 m_projection;

	public:
		CameraGameObject(GAMEOBJECT_INIT_PARAMS);
		~CameraGameObject();

		void UseOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void UsePerspective(float fov, float aspect, float zNear, float zFar);

		glm::mat4 GetView() const;
		const glm::mat4& GetProjection() const;

		void OnFrameResize(const Context::FrameBufferResizeEvent* e);
	};
}

#pragma warning(pop)