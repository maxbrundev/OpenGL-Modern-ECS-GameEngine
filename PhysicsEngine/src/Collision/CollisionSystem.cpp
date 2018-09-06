#include "physics_stdafx.h"

#include "Collision/CollisionSystem.h"
#include "Colliders/SphereColliderComponent.h"
#include "Colliders/BoxColliderComponent.h"
#include "Colliders/Bounds/BoundingSphere.h"
#include "RigidBodyComponent.h"

using namespace Physics::Collision;

CollisionSystem::CollisionSystem() : m_octree(Bounds::AABB(Core::Maths::vec3f(-50.f,-50.f,-50.f), Core::Maths::vec3f(50.f, 50.f, 50.f)))
{
	RegisterEventCallback(&CollisionSystem::OnColliderCreated);
	RegisterEventCallback(&CollisionSystem::OnColliderDestroyed);
}

CollisionSystem::CollisionSystem(Physics::Bounds::AABB& p_region) : m_octree(p_region)
{
	RegisterEventCallback(&CollisionSystem::OnColliderCreated);
	RegisterEventCallback(&CollisionSystem::OnColliderDestroyed);
}

CollisionSystem::~CollisionSystem()
{
	UnregisterAllEventCallbacks();
}

void CollisionSystem::FixedUpdate(const double fixed_dt)
{
	m_octree.Update(fixed_dt);
	std::vector<CollisionInfo> collisions = m_octree.GetCollisions();

	for (CollisionInfo& collision : collisions)
	{
		if (SphereColliderComponent* sphereColliderA = dynamic_cast<SphereColliderComponent*>(collision.colliderA))
		{
			if (SphereColliderComponent* sphereColliderB = dynamic_cast<SphereColliderComponent*>(collision.colliderB))
			{
				ResolveCollision(sphereColliderA, sphereColliderB, collision);
			}
			else if (BoxColliderComponent* boxColliderB = dynamic_cast<BoxColliderComponent*>(collision.colliderB))
			{
				ResolveCollision(boxColliderB, sphereColliderA, collision);
			}
			else
				throw std::invalid_argument("Invalid Collider on entity : " + collision.entityB);
		}
		else if (BoxColliderComponent* boxColliderA = dynamic_cast<BoxColliderComponent*>(collision.colliderA))
		{
			if (SphereColliderComponent* sphereColliderB = dynamic_cast<SphereColliderComponent*>(collision.colliderB))
			{
				ResolveCollision(boxColliderA, sphereColliderB, collision);
			}
			else if (BoxColliderComponent* boxColliderB = dynamic_cast<BoxColliderComponent*>(collision.colliderB))
			{
				ResolveCollision(boxColliderA, boxColliderB, collision);
			}
			else
				throw std::invalid_argument("Invalid Collider on entity : " + collision.entityB);
		}
		else
			throw std::invalid_argument("Invalid Collider on entity : " + collision.entityA);
	}
}

#pragma region CollisionResponse

void CollisionSystem::ResolveCollision(BoxColliderComponent* boxColliderA, SphereColliderComponent* sphereColliderB, const CollisionInfo& collision)
{
	RigidBody::RigidBodyComponent* rbA = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(collision.entityA);
	RigidBody::RigidBodyComponent* rbB = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(collision.entityB);

	const Physics::Bounds::AABB* boundBoxA = dynamic_cast<const Physics::Bounds::AABB*>(boxColliderA->Bounds());

	if (rbA && rbA->HasMoved())
	{
		if (rbB && rbB->HasMoved())
		{
		}
		else
		{
			Core::Transform& transformB = ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(collision.entityB)->AsTransform();

			Core::Maths::vec3f newPos = rbA->Position();
			Core::Maths::vec3f boundBoxCenterA = (boundBoxA->m_max - boundBoxA->m_min) * 0.5f;

			for (int i = 0; i < 3; ++i)
			{
				newPos[i] = (collision.normal[i] > 0.0f) ? transformB.GetPosition()[i] + (sphereColliderB->GetRadius() + boundBoxCenterA[i]) : newPos[i];
				newPos[i] = (collision.normal[i] < 0.0f) ? transformB.GetPosition()[i] - (sphereColliderB->GetRadius() + boundBoxCenterA[i]) : newPos[i];
			}
			rbA->Position(newPos);

			rbA->Velocity().x *= (collision.normal.x != 0.0f) ? 0.0f : 0.1f;
			rbA->Velocity().y *= (collision.normal.y != 0.0f) ? 0.0f : 0.1f;
			rbA->Velocity().z *= (collision.normal.z != 0.0f) ? 0.0f : 0.1f;
		}
	}
	else
	{
		if (rbB && rbB->HasMoved())
		{
			Core::Transform& transformA = ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(collision.entityA)->AsTransform();

			Core::Maths::vec3f newPos = rbB->Position();
			Core::Maths::vec3f boundBoxCenterA = (boundBoxA->m_max - boundBoxA->m_min) * 0.5f;

			for (int i = 0; i < 3; ++i)
			{
				newPos[i] = (collision.normal[i] > 0.0f) ? transformA.GetPosition()[i] - (sphereColliderB->GetRadius() + boundBoxCenterA[i]): newPos[i];
				newPos[i] = (collision.normal[i] < 0.0f) ? transformA.GetPosition()[i] + (sphereColliderB->GetRadius() + boundBoxCenterA[i]): newPos[i];
			}
			rbB->Position(newPos);
			rbB->Velocity().x *= (collision.normal.x != 0.0f) ? 0.0f : 0.9f;
			rbB->Velocity().y *= (collision.normal.y != 0.0f) ? 0.0f : 0.9f;
			rbB->Velocity().z *= (collision.normal.z != 0.0f) ? 0.0f : 0.9f;
		}
	}
}

void CollisionSystem::ResolveCollision(BoxColliderComponent* boxColliderA, BoxColliderComponent* boxColliderB, const CollisionInfo& collision)
{
	RigidBody::RigidBodyComponent* rbA = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(collision.entityA);
	RigidBody::RigidBodyComponent* rbB = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(collision.entityB);

	if (rbA && rbA->HasMoved())
	{
		if (rbB && rbB->HasMoved())
		{
			rbA->Position() += -collision.normal * collision.distance * 0.5f;
			rbB->Position() += collision.normal * collision.distance * 0.5f;

			if (collision.normal.x != 0.0f)				rbA->Velocity().x = 0;
			if (collision.normal.y != 0.0f)				rbA->Velocity().y = 0;
			if (collision.normal.z != 0.0f)				rbA->Velocity().z = 0;

			if (collision.normal.x != 0.0f)				rbB->Velocity().x = 0;
			if (collision.normal.y != 0.0f)				rbB->Velocity().y = 0;
			if (collision.normal.z != 0.0f)				rbB->Velocity().z = 0;
		}
		else
		{
			rbA->Position() += -collision.normal * collision.distance;
			if (collision.normal.x != 0.0f)				rbA->Velocity().x = 0;
			if (collision.normal.y != 0.0f)				rbA->Velocity().y = 0;
			if (collision.normal.z != 0.0f)				rbA->Velocity().z = 0;
		}
	}
	else
	{
		if (rbB && rbB->HasMoved())
		{
			rbB->Position() += collision.normal * collision.distance;
			if (collision.normal.x != 0.0f)				rbB->Velocity().x = 0;
			if (collision.normal.y != 0.0f)				rbB->Velocity().y = 0;
			if (collision.normal.z != 0.0f)				rbB->Velocity().z = 0;
		}
	}
}

void CollisionSystem::ResolveCollision(SphereColliderComponent* sphereColliderA, SphereColliderComponent* sphereColliderB, const CollisionInfo& collision)
{
	RigidBody::RigidBodyComponent* rbA = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(collision.entityA);
	RigidBody::RigidBodyComponent* rbB = ECS::ECS_Engine->GetComponentManager()->GetComponent<RigidBody::RigidBodyComponent>(collision.entityB);

	if (rbA && rbA->HasMoved())
	{
		if(rbB && rbB->HasMoved())
		{
			rbA->Position() += -collision.normal * (sphereColliderA->GetRadius() - collision.distance * 0.5f);
			rbB->Position() += collision.normal * (sphereColliderA->GetRadius() - collision.distance * 0.5f);
		}
		else
		{
			rbA->Position() += -collision.normal * (sphereColliderA->GetRadius() + sphereColliderB->GetRadius() - collision.distance);
		}
	}
	else
	{
		if (rbB && rbB->HasMoved())
		{
			rbB->Position() += collision.normal * (sphereColliderA->GetRadius() + sphereColliderB->GetRadius() - collision.distance);
		}
	}
}

#pragma endregion

#pragma region CollisionDetection

CollisionInfo CollisionSystem::Collides(ACollider* colliderA, ACollider* colliderB)
{
	if(SphereColliderComponent* sphereColliderA  = dynamic_cast<SphereColliderComponent*>(colliderA))
	{
		return Collides(sphereColliderA, colliderB);
	}
	if (BoxColliderComponent* boxColliderA = dynamic_cast<BoxColliderComponent*>(colliderA))
	{
		return Collides(boxColliderA, colliderB);
	}
	throw std::invalid_argument("Invalid Collider");
}

CollisionInfo CollisionSystem::Collides(SphereColliderComponent* sphereColliderA, ACollider* colliderB)
{
	if (SphereColliderComponent* sphereColliderB = dynamic_cast<SphereColliderComponent*>(colliderB))
	{
		return Collides(sphereColliderA, sphereColliderB);
	}
	if (BoxColliderComponent* boxColliderB = dynamic_cast<BoxColliderComponent*>(colliderB))
	{
		return Collides(boxColliderB, sphereColliderA);
	}
	throw std::invalid_argument("Invalid Collider");
}

CollisionInfo CollisionSystem::Collides(BoxColliderComponent* boxColliderA, ACollider* colliderB)
{
	if(BoxColliderComponent* boxColliderB = dynamic_cast<BoxColliderComponent*>(colliderB))
	{
		return Collides(boxColliderA, boxColliderB);
	}
	if (SphereColliderComponent* sphereColliderB = dynamic_cast<SphereColliderComponent*>(colliderB))
	{
		return Collides(boxColliderA, sphereColliderB);
	}
	throw std::invalid_argument("Invalid Collider");
}

CollisionInfo CollisionSystem::Collides(BoxColliderComponent* boxColliderA, SphereColliderComponent* sphereColliderB)
{
	Core::Transform* transformA = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(boxColliderA->GetOwnerID())->AsTransform();
	Core::Transform* transformB = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(sphereColliderB->GetOwnerID())->AsTransform();

	const Physics::Bounds::AABB* boundBoxA = dynamic_cast<const Physics::Bounds::AABB*>(boxColliderA->Bounds());
	const Physics::Bounds::BoundingSphere* boundingSphere = dynamic_cast<const Physics::Bounds::BoundingSphere*>(sphereColliderB->Bounds());

	const Bounds::AABB worldAABB(boundBoxA->m_min + transformA->GetPosition(), boundBoxA->m_max + transformA->GetPosition());
	const Core::Maths::vec3f worldCenter(boundingSphere->m_center + transformB->GetPosition());

	// Find point p on AABB closest to sphere center
	const Core::Maths::vec3f closest = Bounds::AABB::ClosestPtPointAABB(worldAABB, worldCenter);
	// Sphere and AABB intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	Core::Maths::vec3f v = closest - worldCenter;
	if(v.DotProduct(v) <= boundingSphere->m_radius * boundingSphere->m_radius)
	{
		return CollisionInfo(
					Physics::Bounds::ContainmentType::Intersects,
					{closest},
					v,
					boxColliderA,
					sphereColliderB,
					v.Length(),
					boxColliderA->GetOwnerID(),
					sphereColliderB->GetOwnerID()
				);
	}
	return CollisionInfo(
		Physics::Bounds::ContainmentType::Outside,
		{ closest },
		v,
		boxColliderA,
		sphereColliderB,
		v.Length(),
		boxColliderA->GetOwnerID(),
		sphereColliderB->GetOwnerID()
	);
}

CollisionInfo CollisionSystem::Collides(BoxColliderComponent* boxColliderA,	BoxColliderComponent* boxColliderB)
{
	Core::Transform* transformA = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(boxColliderA->GetOwnerID())->AsTransform();
	Core::Transform* transformB = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(boxColliderB->GetOwnerID())->AsTransform();
	const Physics::Bounds::AABB* boundBoxA = dynamic_cast<const Physics::Bounds::AABB*>(boxColliderA->Bounds());
	const Physics::Bounds::AABB* boundBoxB = dynamic_cast<const Physics::Bounds::AABB*>(boxColliderB->Bounds());

	const Core::Maths::vec3f boxAMin	= boundBoxA->m_min + transformA->GetPosition();
	const Core::Maths::vec3f boxAMax	= boundBoxA->m_max + transformA->GetPosition();
	const Core::Maths::vec3f boxBCenter = (boundBoxB->m_max - boundBoxB->m_min) * 0.5f + transformB->GetPosition();
	const Core::Maths::vec3f boxBMin	= boundBoxB->m_min + transformB->GetPosition();
	const Core::Maths::vec3f boxBMax	= boundBoxB->m_max + transformB->GetPosition();

	const Bounds::AABB worldBoxA(boundBoxA->m_min + transformA->GetPosition(), boundBoxA->m_max + transformA->GetPosition());

	Core::Maths::vec3f distances1 = boxBMin - boxAMax;
	const Core::Maths::vec3f distances2 = boxAMin - boxBMax;

	Core::Maths::vec3f distances = distances1.Max(distances2);

	const float maxDistance = distances.Max();

	if(maxDistance <= 0.0f)
	{
		const Core::Maths::vec3f closest = Bounds::AABB::ClosestPtPointAABB(worldBoxA, boxBCenter);
		const Core::Maths::vec3f norm = boxBCenter - closest;

		return CollisionInfo(
			Physics::Bounds::ContainmentType::Intersects,
			{},
			norm,
			boxColliderA,
			boxColliderB,
			maxDistance,
			boxColliderA->GetOwnerID(),
			boxColliderB->GetOwnerID()
		);
			
	}
	return CollisionInfo(
		Physics::Bounds::ContainmentType::Outside,
		{},
		{},
		boxColliderA,
		boxColliderB,
		maxDistance,
		boxColliderA->GetOwnerID(),
		boxColliderB->GetOwnerID()
	);
}

CollisionInfo CollisionSystem::Collides(SphereColliderComponent* sphereColliderA,SphereColliderComponent* sphereColliderB)
{
	Core::Transform* transformA = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(sphereColliderA->GetOwnerID())->AsTransform();
	Core::Transform* transformB = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(sphereColliderB->GetOwnerID())->AsTransform();
	const Physics::Bounds::BoundingSphere* boundingSphereA = dynamic_cast<const Physics::Bounds::BoundingSphere*>(sphereColliderA->Bounds());
	const Physics::Bounds::BoundingSphere* boundingSphereB = dynamic_cast<const Physics::Bounds::BoundingSphere*>(sphereColliderB->Bounds());

	const float radiusSub = sphereColliderA->GetRadius() - sphereColliderB->GetRadius();
	const float radiusSum = sphereColliderA->GetRadius() + sphereColliderB->GetRadius();

	const float radiusSqSub = radiusSub * radiusSub;
	const float radiusSqSum = radiusSum * radiusSum;

	const Core::Maths::vec3f sphereCenterA = boundingSphereA->m_center + transformA->GetPosition();
	const Core::Maths::vec3f sphereCenterB = boundingSphereB->m_center + transformB->GetPosition();
	
	Core::Maths::vec3f dist = sphereCenterB - sphereCenterA;

	if (dist.MagSq() < radiusSqSub)
		return CollisionInfo(
			Physics::Bounds::ContainmentType::Contains,
			{},
			{},
			sphereColliderA,
			sphereColliderB,
			dist.Length(),
			sphereColliderA->GetOwnerID(),
			sphereColliderB->GetOwnerID()
			);
	if (dist.MagSq() <= radiusSqSum)
	{
		return CollisionInfo(
			Physics::Bounds::ContainmentType::Intersects,
			{},
			dist,
			sphereColliderA,
			sphereColliderB,
			dist.Length(),
			sphereColliderA->GetOwnerID(),
			sphereColliderB->GetOwnerID()
		);
	}
	return CollisionInfo(
		Physics::Bounds::ContainmentType::Outside,
		{},
		{},
		sphereColliderA,
		sphereColliderB,
		dist.Length(),
		sphereColliderA->GetOwnerID(),
		sphereColliderB->GetOwnerID()
	);
}

Physics::Bounds::ContainmentType CollisionSystem::Contains(Physics::Bounds::AABB* p_aabb, ACollider* colliderB)
{
	if (BoxColliderComponent* boxColliderB = dynamic_cast<BoxColliderComponent*>(colliderB))
	{
		Core::Transform* transformB = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(boxColliderB->GetOwnerID())->AsTransform();

		const Physics::Bounds::AABB* boundBoxB = dynamic_cast<const Physics::Bounds::AABB*>(boxColliderB->Bounds());

		const Core::Maths::vec3f& boxAMin = p_aabb->m_min;
		const Core::Maths::vec3f& boxAMax = p_aabb->m_max;
		const Core::Maths::vec3f& boxBMin = boundBoxB->m_min + transformB->GetPosition();
		const Core::Maths::vec3f& boxBMax = boundBoxB->m_max + transformB->GetPosition();

		if (boxAMin.x < boxBMin.x && boxAMin.y < boxBMin.y && boxAMin.z < boxBMin.z)
		{
			if (boxAMax.x > boxBMax.x && boxAMax.y > boxBMax.y && boxAMax.z > boxBMax.z)
			{
				return Physics::Bounds::ContainmentType::Contains;
			}
			if (boxAMax.x < boxBMax.x && boxAMax.y < boxBMax.y && boxAMax.z < boxBMax.z)
				return Physics::Bounds::ContainmentType::Outside;
			return Physics::Bounds::ContainmentType::Intersects;
		}
		if (boxBMax.x > boxAMin.x && boxBMax.y < boxAMin.y && boxBMax.z < boxAMin.z)
			return Physics::Bounds::ContainmentType::Outside;
		return Physics::Bounds::ContainmentType::Intersects;
	}
	if (SphereColliderComponent* sphereColliderB = dynamic_cast<SphereColliderComponent*>(colliderB))
	{
		Core::Transform* transformB = &ECS::ECS_Engine->GetComponentManager()->GetComponent<Core::TransformComponent>(sphereColliderB->GetOwnerID())->AsTransform();
		const Physics::Bounds::BoundingSphere* boundingSphere = dynamic_cast<const Physics::Bounds::BoundingSphere*>(sphereColliderB->Bounds());

		const Core::Maths::vec3f sphereCenter = boundingSphere->m_center + transformB->GetPosition();

		const Core::Maths::vec3f& boxAMin = p_aabb->m_min;
		const Core::Maths::vec3f& boxAMax = p_aabb->m_max;

		float dmin = 0;
		int inside = 0;
		const float radiusSq = sphereColliderB->GetRadius() * sphereColliderB->GetRadius();

		for (unsigned int i = 0; i < 3; i++)
		{
			if (sphereCenter[i] >= boxAMin[i] + sphereColliderB->GetRadius() && sphereCenter[i] <= boxAMax[i] - sphereColliderB->GetRadius())
				inside++;
			if (sphereCenter[i] < boxAMin[i])
				dmin += (sphereCenter[i] - boxAMin[i]) * (sphereCenter[i] - boxAMin[i]);
			else if (sphereCenter[i] > boxAMax[i])
				dmin += (sphereCenter[i] - boxAMax[i]) * (sphereCenter[i] - boxAMax[i]);
		}

		if (inside == 3)
		{
			return Bounds::ContainmentType::Contains;
		}
		if (dmin <= radiusSq)
			return Bounds::ContainmentType::Intersects;
		return Bounds::ContainmentType::Outside;
	}
	throw std::invalid_argument("Invalid Collider");
}

#pragma endregion

void CollisionSystem::OnColliderCreated(const ColliderCreated* e)
{
	m_octree.AddEntity(e->collider);
}
void CollisionSystem::OnColliderDestroyed(const ColliderDestroyed* e)
{
	m_octree.AddEntity(e->collider);
}