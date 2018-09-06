#include "physics_stdafx.h"

#include "Colliders/BoxColliderComponent.h"
#include "Colliders/Bounds/AABB.h"

using namespace Physics::Collision;
using namespace Physics::Bounds;

BoxColliderComponent::BoxColliderComponent(COMPONENT_INIT_PARAMS, const Core::Maths::vec3f& p_boundmin, 
	const Core::Maths::vec3f& p_boundMax, const Core::Transform& p_transform) 
: ACollider(p_transform, new AABB(p_boundmin, p_boundMax), false), COMPONENT_INIT(BoxColliderComponent)
{}

BoxColliderComponent::~BoxColliderComponent()
{
	delete m_bounds;
}