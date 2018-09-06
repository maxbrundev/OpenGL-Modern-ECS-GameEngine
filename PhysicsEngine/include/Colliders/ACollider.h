#pragma once

#include "PhysicsAPI.h"

#include "Colliders/ColliderEvents.h"

namespace Physics 
{
	namespace Bounds 
	{
		class IBounds;
	}
}

namespace Physics
{
	namespace Collision
	{
		class PHYSICS_API ACollider
		{
		protected:
			const Core::Transform& m_transform;
			Bounds::IBounds* m_bounds;
			bool m_isTrigger;

		public:
			ACollider(const Core::Transform& p_transform, Bounds::IBounds* p_bounds, const bool p_isTrigger) : m_transform(p_transform), m_bounds(p_bounds), m_isTrigger(p_isTrigger)
			{
				ECS::ECS_Engine->SendEvent<ColliderCreated>(this);
			}
			virtual ~ACollider()
			{
				ECS::ECS_Engine->SendEvent<ColliderDestroyed>(this);
			}

			virtual const Bounds::IBounds* Bounds() const { return m_bounds; }
		};
	}
}