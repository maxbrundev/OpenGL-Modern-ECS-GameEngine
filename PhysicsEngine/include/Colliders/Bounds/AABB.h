#pragma once

#include "PhysicsAPI.h"

#include "Colliders/Bounds/IBounds.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace Bounds
	{
		struct PHYSICS_API AABB : public IBounds
		{
			Core::Maths::vec3f m_min;
			Core::Maths::vec3f m_max;

			AABB();
			AABB(const Core::Maths::vec3f& p_min, const Core::Maths::vec3f& p_max);
			~AABB();

			// Given point p, return the point q on or in AABB b that is closest to p
			static Core::Maths::vec3f ClosestPtPointAABB(const AABB& p_aabb, const Core::Maths::vec3f& p);
		};
	}
}

#pragma warning(pop)