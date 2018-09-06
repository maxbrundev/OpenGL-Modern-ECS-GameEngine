#include "physics_stdafx.h"

#include "Colliders/Bounds/BoundingSphere.h"

using namespace Physics::Bounds;

BoundingSphere::BoundingSphere(const Core::Maths::vec3f& p_center, const float p_radius) : m_center(p_center), m_radius(p_radius)
{}

BoundingSphere::BoundingSphere(const float p_radius): m_radius(p_radius)
{}

BoundingSphere::BoundingSphere(): m_radius(0)
{}

BoundingSphere::~BoundingSphere()
{}