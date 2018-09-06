#include "physics_stdafx.h"

#include "Colliders/SphereColliderComponent.h"
#include "Colliders/Bounds/BoundingSphere.h"

using namespace Physics::Collision;
using namespace Core::Maths;

SphereColliderComponent::SphereColliderComponent(COMPONENT_INIT_PARAMS, const float p_radius, const Core::Transform& p_transform) : 
ACollider(p_transform, new Bounds::BoundingSphere(p_radius), false), COMPONENT_INIT(SphereColliderComponent)
{}

SphereColliderComponent::~SphereColliderComponent()
{
	delete m_bounds;
}

float SphereColliderComponent::GetRadius() const
{
	return static_cast<Bounds::BoundingSphere*>(m_bounds)->m_radius;
}