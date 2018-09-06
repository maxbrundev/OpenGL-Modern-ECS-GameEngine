#include "physics_stdafx.h"

#include "PhysicsSystem.h"
#include "RigidBodyComponent.h"

using namespace Physics;
using namespace RigidBody;

float PhysicsSystem::DeadZoneHeight = -15.0f;
PhysicsSystem::PhysicsSystem()
{}

PhysicsSystem::PhysicsSystem(const glm::vec3& p_gravity) : m_gravity(p_gravity)
{}

PhysicsSystem::~PhysicsSystem()
{}

void PhysicsSystem::FixedUpdate(const double fixed_dt)
{
   	for(auto rigidBodyIt = ECS::ECS_Engine->GetComponentManager()->GetComponentContainer<RigidBodyComponent>()->GetTComponentContainer().begin();
		rigidBodyIt != ECS::ECS_Engine->GetComponentManager()->GetComponentContainer<RigidBodyComponent>()->GetTComponentContainer().end();
		++rigidBodyIt)
	{
		RigidBodyComponent* rigidBody = reinterpret_cast<RigidBodyComponent*>(rigidBodyIt->second.get());
		if(ECS::ECS_Engine->GetEntityManager()->GetEntity(rigidBody->GetOwnerID())->IsActive())
		{
			Core::Transform& transform = ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(rigidBody->GetOwnerID())->AsTransform();
			if (rigidBody->Position() != transform.GetPosition())
				transform.SetPosition(rigidBody->Position().ToGLMVec3());
			rigidBody->HasMoved(false);

			if(rigidBody->UseGravity())
			{
				if (transform.GetPosition().y < DeadZoneHeight)
					ECS::ECS_Engine->GetEntityManager()->GetEntity(rigidBody->GetOwnerID())->SetActive(false);

				rigidBody->Addforce(m_gravity);
			}

			rigidBody->Velocity() += rigidBody->Acceleration() * static_cast<float>(fixed_dt);
			rigidBody->Position() += rigidBody->Velocity() * static_cast<float>(fixed_dt);

			if (rigidBody->Position() != transform.GetPosition())
				rigidBody->HasMoved(true);

			transform.SetPosition(rigidBody->Position().ToGLMVec3());
			rigidBody->Acceleration(Core::Maths::vec3f());
		}
	}
}