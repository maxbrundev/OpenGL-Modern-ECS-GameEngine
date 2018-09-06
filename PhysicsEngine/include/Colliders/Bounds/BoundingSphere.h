#pragma once

#include "Colliders/Bounds/IBounds.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace Bounds
	{
		struct PHYSICS_API BoundingSphere : public IBounds
		{

			Core::Maths::vec3f	m_center;
			float				m_radius;

			BoundingSphere(const Core::Maths::vec3f& p_center, float p_radius);
			explicit BoundingSphere(float p_radius);
			BoundingSphere();
			~BoundingSphere();
		};
	}
}

#pragma warning(pop)