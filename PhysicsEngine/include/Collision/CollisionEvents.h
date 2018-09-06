#pragma once

#include "CollisionInfo.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace Collision
	{
		struct CORE_API SphereCollisionEvent : public ECS::Event::Event<SphereCollisionEvent>
		{
			CollisionInfo collisionInfo;
			explicit SphereCollisionEvent(const CollisionInfo&& info) : collisionInfo(std::move(info))
			{}
		};
	}
}

#pragma warning(pop)