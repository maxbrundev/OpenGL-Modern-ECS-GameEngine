#pragma once

#include "physics_stdafx.h"
#include "PhysicsAPI.h"

#include "Collision/Octree.h"
#include "Colliders/Bounds/BoundingSphere.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace Collision
	{
		class BoxColliderComponent;
		class SphereColliderComponent;

		class PHYSICS_API CollisionSystem : public ECS::System<CollisionSystem>, ECS::Event::IEventListener
		{
			Octree m_octree;

		public:
			CollisionSystem();
			explicit CollisionSystem(Bounds::AABB& p_region);
			~CollisionSystem();
			void FixedUpdate(double fixed_dt) override;

			static void ResolveCollision(BoxColliderComponent* boxColliderA, SphereColliderComponent* sphereColliderA, const CollisionInfo& collision);
			static void ResolveCollision(BoxColliderComponent* boxColliderA, BoxColliderComponent* boxColliderB, const CollisionInfo& collision);
			static void ResolveCollision(SphereColliderComponent* sphereColliderA, SphereColliderComponent* sphereColliderB, const CollisionInfo& collision);

			static CollisionInfo Collides(ACollider* colliderA, ACollider* colliderB);
			static CollisionInfo Collides(SphereColliderComponent* sphereColliderA, ACollider* colliderB);
			static CollisionInfo Collides(BoxColliderComponent* boxColliderA, ACollider* colliderB);
			static CollisionInfo Collides(BoxColliderComponent* boxColliderA, SphereColliderComponent* sphereColliderB);
			static CollisionInfo Collides(BoxColliderComponent* boxColliderA, BoxColliderComponent* boxColliderB);
			static CollisionInfo Collides(SphereColliderComponent* sphereColliderA, SphereColliderComponent* sphereColliderB);

			static Bounds::ContainmentType Contains(Bounds::AABB* p_aabb, ACollider* colliderB);

			void OnColliderCreated(const ColliderCreated* e);
			void OnColliderDestroyed(const ColliderDestroyed* e);
		};
	}
}

#pragma warning(pop)