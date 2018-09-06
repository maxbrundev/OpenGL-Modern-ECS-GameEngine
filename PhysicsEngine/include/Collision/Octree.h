#pragma once

#include "physics_stdafx.h"
#include "PhysicsAPI.h"

#include "Colliders/Bounds/AABB.h"
#include "Colliders/ACollider.h"
#include "Collision/CollisionInfo.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Physics
{
	namespace Collision
	{
		class CollisionSystem;

		class PHYSICS_API Octree
		{
		private:
			std::vector<ACollider*> m_objects;
			static std::queue<ACollider*> m_pendingInsertionObjects;
			CollisionSystem* m_collisionSystem;

			Bounds::AABB m_region;

			Octree* m_childs[8]				= {nullptr};
			Octree* m_parent				=  nullptr;

			const int MIN_REGION_SIZE		= 1;

			int m_maxLifetime				= 8;
			int m_currentLife				= -1;
			///Bitmask
			unsigned char m_activeChilds	= 0;

		public:
			static bool m_treeReady;
			static bool m_treeBuilt;

			explicit Octree(const Bounds::AABB& p_region);
			Octree();
			~Octree();

			void Update(double dt_ms);			

			static void AddEntity(ACollider* p_object);
			static Bounds::AABB FindEnclosingCube(Bounds::AABB p_region);
			std::vector<CollisionInfo> GetCollisions();

		private:
			Octree(const Bounds::AABB& p_region, const std::vector<ACollider*>& p_objectList);

			std::vector<CollisionInfo> GetCollisions(std::vector<ACollider*> parentObjs);

			void UpdateTree();
			void BuildTree();
			void Insert(ACollider*  p_item);

			Octree* CreateNode(const Bounds::AABB& p_region, const std::vector<ACollider*>& p_objectList);
			Octree* CreateNode(const Bounds::AABB& p_region, ACollider* p_item);

			void DeleteNodes();
		};
	}
}

#pragma warning(pop)