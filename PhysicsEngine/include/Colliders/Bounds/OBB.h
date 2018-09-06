#pragma once
#include "Colliders/Bounds/IBounds.h"

namespace Physics
{
	namespace Bounds
	{
		struct AABB;

		struct OBB : public IBounds
		{
			union
			{
				glm::vec3 m_bounds[8];
				struct
				{
					glm::vec3 topLeftBack;
					glm::vec3 topLeftFront;
					glm::vec3 topRightBack;
					glm::vec3 topRightFront;

					glm::vec3 bottomLeftBack;
					glm::vec3 bottomLeftFront;
					glm::vec3 bottomRightBack;
					glm::vec3 bottomRightFront;
				};
			};

			OBB() = default;
			~OBB() = default;
		};
	}
}