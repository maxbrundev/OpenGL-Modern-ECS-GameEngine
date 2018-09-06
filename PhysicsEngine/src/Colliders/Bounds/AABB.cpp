#include "physics_stdafx.h"

#include "Colliders/Bounds/AABB.h"

using namespace Physics::Bounds;

AABB::AABB()
{
}

AABB::AABB(const Core::Maths::vec3f& p_min, const Core::Maths::vec3f& p_max) : m_min(p_min), m_max(p_max)
{
}

AABB::~AABB()
{
}

Core::Maths::vec3f AABB::ClosestPtPointAABB(const AABB& p_aabb, const Core::Maths::vec3f& p)
{
	Core::Maths::vec3f closestPt;
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		v = std::max(v, p_aabb.m_min[i]);
		v = std::min(v, p_aabb.m_max[i]);
		closestPt[i] = v;
	}
	return closestPt;
}