#pragma once

#include "physics_stdafx.h"
#include "PhysicsAPI.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace RigidBody
	{
		class PHYSICS_API RigidBodyComponent : public ECS::Component<RigidBodyComponent>
		{
		private:
			Core::Maths::vec3f m_acceleration;
			Core::Maths::vec3f m_velocity;
			Core::Maths::vec3f m_position;

			float m_mass;
			float m_friction;

			bool isGrounded;
			bool useGravity;
			bool hasMoved;

		public:
			RigidBodyComponent(COMPONENT_INIT_PARAMS,float p_mass, float p_friction, bool p_useGravity);
			~RigidBodyComponent() = default;

			void Addforce(const Core::Maths::vec3f& p_force) noexcept;

			Core::Maths::vec3f& Acceleration(const Core::Maths::vec3f& p_acceleration);
			Core::Maths::vec3f& Acceleration();
			const Core::Maths::vec3f& Acceleration() const;

			Core::Maths::vec3f& Velocity(const Core::Maths::vec3f& p_velocity);
			Core::Maths::vec3f& Velocity();
			const Core::Maths::vec3f& Velocity() const;

			Core::Maths::vec3f& Position(const Core::Maths::vec3f& p_position);
			Core::Maths::vec3f& Position();
			const Core::Maths::vec3f& Position() const;

			float& Mass(float p_mass);
			float& Mass();
			float Mass() const;

			float& Friction(float p_friction);
			float& Friction();
			float Friction() const;

			bool HasMoved() const;
			void HasMoved(bool value);

			bool UseGravity() const;
		};
	}
}

#pragma warning(pop)