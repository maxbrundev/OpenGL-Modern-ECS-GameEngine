#pragma once

#include "PhysicsAPI.h"

#include "Colliders/ACollider.h"
#include "Colliders/Bounds/IBounds.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace Collision
	{

		class PHYSICS_API BoxColliderComponent : public ACollider, public ECS::Component<BoxColliderComponent>
		{

		public:
			BoxColliderComponent(COMPONENT_INIT_PARAMS, const Core::Maths::vec3f& p_boundmin, const Core::Maths::vec3f& p_boundMax, const Core::Transform& p_transform);
			~BoxColliderComponent() ;
		};
	}
}