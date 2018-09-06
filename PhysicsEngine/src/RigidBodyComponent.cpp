#include "physics_stdafx.h"

#include "RigidBodyComponent.h"

using namespace Physics;
using namespace RigidBody;

RigidBodyComponent::RigidBodyComponent(COMPONENT_INIT_PARAMS, const float p_mass, const float p_friction, const bool p_useGravity) :
	COMPONENT_INIT(RigidBodyComponent),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_mass(p_mass),
	m_friction(p_friction),
	isGrounded(false),
	useGravity(p_useGravity),
	hasMoved(false)
{
	Core::TransformComponent* transform = ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(m_owner_ID);
	if (transform)
		m_position = transform->AsTransform().GetPosition();
}

void RigidBodyComponent::Addforce(const Core::Maths::vec3f& p_force) noexcept
{
	m_acceleration += (p_force / m_mass);
}

Core::Maths::vec3f& RigidBodyComponent::Acceleration(const Core::Maths::vec3f& p_acceleration)
{
	m_acceleration = p_acceleration;
	return m_acceleration;
}

Core::Maths::vec3f& RigidBodyComponent::Acceleration()
{
	return m_acceleration;
}

const Core::Maths::vec3f& RigidBodyComponent::Acceleration() const
{
	return m_acceleration;
}

Core::Maths::vec3f& RigidBodyComponent::Velocity(const Core::Maths::vec3f& p_velocity)
{
	m_velocity = p_velocity;
	return m_velocity;
}

Core::Maths::vec3f& RigidBodyComponent::Velocity()
{
	return m_velocity;
}

const Core::Maths::vec3f& RigidBodyComponent::Velocity() const
{
	return m_velocity;
}

Core::Maths::vec3f& RigidBodyComponent::Position(const Core::Maths::vec3f& p_position)
{
	m_position = p_position;
	return  m_position;
}

Core::Maths::vec3f& RigidBodyComponent::Position()
{
	return  m_position;
}

const Core::Maths::vec3f& RigidBodyComponent::Position() const
{
	return  m_position;
}

float& RigidBodyComponent::Mass(const float p_mass)
{
	m_mass = p_mass;
	return m_mass;
}

float& RigidBodyComponent::Mass()
{
	return m_mass;
}

float RigidBodyComponent::Mass() const
{
	return m_mass;
}

float& RigidBodyComponent::Friction(const float p_friction)
{
	m_friction = p_friction;
	return m_friction;
}

float& RigidBodyComponent::Friction()
{
	return m_friction;
}

float RigidBodyComponent::Friction() const
{
	return m_friction;
}

bool RigidBodyComponent::HasMoved() const
{
	return hasMoved;
}

void RigidBodyComponent::HasMoved(const bool value)
{
	hasMoved = value;
}

bool RigidBodyComponent::UseGravity() const
{
	return useGravity;
}