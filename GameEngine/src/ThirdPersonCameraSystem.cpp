#include "game_engine_stdafx.h"

#include "ThirdPersonCameraSystem.h"

using namespace Camera;

ThirdPersonCameraSystem::ThirdPersonCameraSystem()
{
	RegisterEventCallback(&ThirdPersonCameraSystem::OnMouseMoved);
}

ThirdPersonCameraSystem::ThirdPersonCameraSystem(const ECS::EntityID p_cameraId, const ECS::EntityID p_targetId) :
	m_camera(dynamic_cast<Renderer::CameraGameObject*>(ECS::ECS_Engine->GetEntityManager()->GetEntity(p_cameraId))),
	m_cameraTransform(
		&ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(p_cameraId)->AsTransform()),
	m_target(&ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(p_targetId)->AsTransform())

{
	RegisterEventCallback(&ThirdPersonCameraSystem::OnMouseMoved);

	m_offset = m_cameraTransform->GetPosition() - m_target->GetPosition();
	m_radius = static_cast<float>(m_offset.length());
	m_theta = atan2(m_offset.y, m_offset.x);
	m_phi = atan2(sqrt(m_offset.x * m_offset.x + m_offset.y * m_offset.y), m_radius);
}

void ThirdPersonCameraSystem::Update(double fixed_dt)
{
	m_cameraTransform->SetPosition(m_target->GetPosition() + GetCartesianCoordinates());

	m_cameraTransform->LookAt(m_target->GetPosition(), glm::vec3(0, m_up, 0));
	m_target->SetOrientation(0, m_cameraTransform->GetOrientation().z + m_cameraTransform->GetOrientation().y, m_cameraTransform->GetOrientation().z);
}

void ThirdPersonCameraSystem::SetCamera(const ECS::EntityID p_id)
{
	m_camera = dynamic_cast<Renderer::CameraGameObject*>(ECS::ECS_Engine->GetEntityManager()->GetEntity(p_id));
	m_cameraTransform = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(p_id)->AsTransform();

	if (m_target && m_camera)
	{
		m_offset = m_cameraTransform->GetPosition() - m_target->GetPosition();
		m_radius = static_cast<float>(m_offset.length());
		m_theta = atan2(m_offset.y, m_offset.x);
		m_phi = atan2(sqrt(m_offset.x * m_offset.x + m_offset.y * m_offset.y), m_radius);
	}
}

void ThirdPersonCameraSystem::SetTarget(const ECS::EntityID p_id)
{
	m_target = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(p_id)->AsTransform();

	if (m_target && m_camera)
	{
		m_offset = m_cameraTransform->GetPosition() - m_target->GetPosition();
		m_radius = static_cast<float>(m_offset.length());
		m_theta = atan2(m_offset.y, m_offset.x);
		m_phi = atan2(sqrt(m_offset.x * m_offset.x + m_offset.y * m_offset.y), m_radius);
	}
}

void ThirdPersonCameraSystem::Rotate(const float dTheta, const float dPhi)
{
	if (m_up > 0.0f)
	{
		m_theta += dTheta;
	}
	else
	{
		m_theta -= dTheta;
	}

	m_phi += dPhi;

//	m_theta = fmod(m_theta, static_cast<float>(2 * M_PI));
	m_phi = static_cast<float>(std::clamp<double>(m_phi, 0.01f, M_PI_2));

	if ((m_phi > 0 && m_phi < M_PI) || (m_phi < -M_PI && m_phi > -2 * M_PI))
	{
		m_up = 1.0f;
	}
	else
	{
		m_up = -1.0f;
	}
}

glm::vec3 ThirdPersonCameraSystem::GetCartesianCoordinates() const
{
	return m_radius * glm::vec3(
		m_radius * sinf(m_phi) * sinf(m_theta),
		m_radius * cosf(m_phi),
		m_radius * sinf(m_phi) * cosf(m_theta));
}

void ThirdPersonCameraSystem::OnMouseMoved(const Core::Events::MouseMoved* e)
{
	Rotate(e->xPos * 0.05f * static_cast<float>(Utils::Timer::DeltaTime().count()), e->yPos * 0.05f * static_cast<float>(Utils::Timer::DeltaTime().count()));
}