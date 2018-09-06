#pragma once

#include "Colliders/ACollider.h"

namespace Physics
{
	namespace Collision
	{
		class PHYSICS_API SphereColliderComponent : public ACollider, public ECS::Component<SphereColliderComponent>
		{
		public:
			SphereColliderComponent(COMPONENT_INIT_PARAMS, float p_radius, const Core::Transform& p_transform);
			~SphereColliderComponent();

			float GetRadius() const;

		};
	}
}