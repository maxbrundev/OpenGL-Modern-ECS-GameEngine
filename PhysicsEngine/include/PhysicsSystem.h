#pragma once

#include "physics_stdafx.h"
#include "PhysicsAPI.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	class PHYSICS_API PhysicsSystem : public ECS::System<PhysicsSystem>
	{
	private:
		glm::vec3 m_gravity {0.0f,-1.0f,0.0f};

	public:
		static float DeadZoneHeight;

		PhysicsSystem();
		explicit PhysicsSystem(const glm::vec3& p_gravity);
		~PhysicsSystem();

		void FixedUpdate(double fixed_dt) override;

		const glm::vec3& Gravity() const { return m_gravity; }
	};
}

#pragma warning(pop)