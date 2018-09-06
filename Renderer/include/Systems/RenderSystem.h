#pragma once

#include "renderer_stdafx.h"

#include "RendererAPI.h"
#include <Entities/CameraGameObject.h>

namespace Renderer
{
	class RENDERER_API RenderSystem : public ECS::System<RenderSystem>
	{
	private:
		GLFWwindow* m_window;
		CameraGameObject* m_activeCamera;

		void InitOpenGL() const;
	public:
		explicit RenderSystem(GLFWwindow* p_window);
		~RenderSystem() = default;

		void PreUpdate(double dt) override;

		void Update(double dt) override;

		void PostUpdate(double dt) override;

		void SetCamera(CameraGameObject* p_camera);
	};
}