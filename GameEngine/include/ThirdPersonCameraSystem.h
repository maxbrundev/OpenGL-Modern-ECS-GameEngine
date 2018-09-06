#pragma once

#include "game_engine_stdafx.h"

namespace Camera
{
	class ThirdPersonCameraSystem : public ECS::System<ThirdPersonCameraSystem>, ECS::Event::IEventListener
	{
	private:
		Renderer::CameraGameObject* m_camera {nullptr};

		Core::Transform* m_cameraTransform { nullptr };
		Core::Transform* m_target { nullptr };

		float m_radius { 1.0f };
		float m_theta;
		float m_phi;
		float m_up { 1.0f };

		glm::vec3 m_offset;

	public:
		ThirdPersonCameraSystem();
		ThirdPersonCameraSystem(ECS::EntityID p_cameraId, ECS::EntityID p_targetId);
		~ThirdPersonCameraSystem() = default;

		void Update(double fixed_dt) override;

		void SetCamera(ECS::EntityID p_id);
		void SetTarget(ECS::EntityID p_id);

		void Rotate(float dTheta, float dPhi);
		glm::vec3 GetCartesianCoordinates() const;

		void OnMouseMoved(const Core::Events::MouseMoved* e);
	};
}