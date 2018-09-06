#pragma once

#include "physics_stdafx.h"
#include "PhysicsAPI.h"

namespace Physics
{
	namespace Bounds
	{
		struct OBB;
		struct AABB;
		struct BoundingSphere;

		enum class ContainmentType : uint8_t
		{
			Contains,
			Intersects,
			Outside
		};

		class PHYSICS_API IBounds
		{
		public:
			virtual ~IBounds() = default;
		};
	}
}