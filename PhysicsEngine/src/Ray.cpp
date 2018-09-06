#include "physics_stdafx.h"

#include "Ray.h"
#include "Colliders/Bounds/AABB.h"

using namespace Physics;

Ray::Ray(const Core::Maths::vec3f& p_origin, const Core::Maths::vec3f& p_direction) : m_origin(p_origin), m_direction(p_direction)
{}

Ray::~Ray()
{}

Core::Maths::vec3f Ray::GetPoint(const float p_distance) const
{
	return m_origin + p_distance * m_direction;
}

Collision::CollisionInfo Ray::IntersectRaySphere(const Ray& p_ray,
	const Bounds::BoundingSphere& p_sphere)
{
	{
		Core::Maths::vec3f m = p_ray.m_origin - p_sphere.m_center;

		const float b = m.DotProduct(p_ray.m_direction);
		const float c = m.DotProduct(m) - (p_sphere.m_radius * p_sphere.m_radius);

		// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
		if (c > 0.0f && b > 0.0f)
			return Collision::CollisionInfo(
				Bounds::ContainmentType::Outside,
				{},
				{},
				nullptr,
				nullptr,
				-1,
				999,
				999
			);
		const float discr = b * b - c;
		// A negative discriminant corresponds to ray missing sphere
		if (discr < 0.0f) 
			return Collision::CollisionInfo(
				Bounds::ContainmentType::Outside,
				{},
				{},
				nullptr,
				nullptr,
				-1,
				999,
				999
			);
		// Ray now found to intersect sphere, compute smallest t value of intersection
		float t = -b - std::sqrt(discr);
		// If t is negative, ray started inside sphere so clamp t to zero
		if (t < 0.0f) t = 0.0f;
		const Core::Maths::vec3f intersectionPoint = p_ray.m_origin + t * p_ray.m_direction;
		return Collision::CollisionInfo(
			Bounds::ContainmentType::Intersects,
			{ intersectionPoint },
			intersectionPoint - p_ray.m_origin,
			nullptr,
			nullptr,
			t,
			999,
			999
		);
	}
}

Collision::CollisionInfo Ray::IntersectRayAABB(const Ray& p_ray, const Bounds::AABB& p_aabb)
{
	float tmin = 0.0f; // set to -FLT_MAX to get first hit on line
	float tmax = FLT_MAX; // set to max distance ray can travel (for segment)

	// For all three slabs
	for (int i = 0; i < 3; i++) {
		if (std::abs(p_ray.m_direction[i]) < std::numeric_limits<float>::epsilon()) {
			// Ray is parallel to slab. No hit if origin not within slab
			if (p_ray.m_origin[i] < p_aabb.m_min[i] || p_ray.m_origin[i] > p_aabb.m_max[i]) 
				return Collision::CollisionInfo(
					Bounds::ContainmentType::Outside,
					{},
					{},
					nullptr,
					nullptr,
					-1,
					999,
					999
				);
		}
		else {
			// Compute intersection t value of ray with near and far plane of slab
			const float ood = 1.0f / p_ray.m_direction[i];
			float t1 = (p_aabb.m_min[i] - p_ray.m_origin[i]) * ood;
			float t2 = (p_aabb.m_max[i] - p_ray.m_origin[i]) * ood;

			// Make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2) std::swap(t1, t2);
			// Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 > tmax) tmax = t2;
			// Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax)
				return Collision::CollisionInfo(
					Bounds::ContainmentType::Outside,
					{},
					{},
					nullptr,
					nullptr,
					-1,
					999,
					999
				);
		}
	}
	// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	const Core::Maths::vec3f intersectionPoint = p_ray.m_origin + p_ray.m_direction * tmin;
	return Collision::CollisionInfo(
		Bounds::ContainmentType::Intersects,
		{ intersectionPoint },
		intersectionPoint - p_ray.m_origin,
		nullptr,
		nullptr,
		tmin,
		999,
		999
	);
}