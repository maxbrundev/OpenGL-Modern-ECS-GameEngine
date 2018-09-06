#pragma once

#include "PhysicsAPI.h"

namespace Physics
{
	namespace Collision
	{
		class ACollider;

		struct PHYSICS_API ColliderCreated : public ECS::Event::Event<ColliderCreated>
		{
			ACollider* collider;

			explicit ColliderCreated(ACollider* p_collider) :
				collider(p_collider)
			{}
		};

		struct PHYSICS_API ColliderDestroyed : public ECS::Event::Event<ColliderDestroyed>
		{
			ACollider* collider;

			explicit ColliderDestroyed(ACollider* p_collider) :
				collider(p_collider)
			{}
		};
	}
}