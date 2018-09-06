#pragma once

#include "Collision/CollisionInfo.h"
#include "Colliders/Bounds/BoundingSphere.h"

namespace Physics
{
	class Ray
	{
	private:
		Core::Maths::vec3f m_origin;
		Core::Maths::vec3f m_direction;

	public:
		Ray(const Core::Maths::vec3f& p_origin, const Core::Maths::vec3f& p_direction);
		~Ray();

		Core::Maths::vec3f GetPoint(float p_distance) const;

		// Intersects ray r = p + td, |d| = 1, with sphere s and, if intersecting,
		// returns t value of intersection and intersection point q
		static Collision::CollisionInfo IntersectRaySphere(const Ray& p_ray, const Bounds::BoundingSphere& p_sphere);
		static Collision::CollisionInfo IntersectRayAABB(const Ray& p_ray, const Bounds::AABB& p_aabb);
	};
}