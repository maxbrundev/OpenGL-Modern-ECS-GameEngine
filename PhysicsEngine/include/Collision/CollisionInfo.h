#pragma once

#include "physics_stdafx.h"

#include "Colliders/Bounds/IBounds.h"

namespace Physics
{
	namespace Collision
	{
		class ACollider;

		struct CollisionInfo
		{
			Bounds::ContainmentType			containmentType { Bounds::ContainmentType::Outside };
			std::vector<Core::Maths::vec3f>	contacts		{};
			Core::Maths::vec3f				normal			{};
			ACollider*						colliderA		{ nullptr };
			ACollider*						colliderB		{ nullptr };
			float							distance		{	 0	  };
			ECS::EntityID					entityA			{   999	  };
			ECS::EntityID					entityB			{	999	  };

			CollisionInfo() = default;

			CollisionInfo(const Bounds::ContainmentType	p_containmentType, const std::vector<Core::Maths::vec3f>& p_contacts, const Core::Maths::vec3f& p_normal,ACollider* p_colliderA,  ACollider* p_colliderB, const float p_distance, const ECS::EntityID p_entityA, const ECS::EntityID p_entityB) :
					containmentType(p_containmentType), contacts(p_contacts), normal(p_normal), colliderA(p_colliderA), colliderB(p_colliderB), distance(p_distance), entityA(p_entityA), entityB(p_entityB)
			{
				normal.Normalize();
			}

			CollisionInfo(const CollisionInfo& other) noexcept :
				containmentType(other.containmentType),
				contacts(other.contacts), 
				normal(other.normal), 
				colliderA(other.colliderA), 
				colliderB(other.colliderB), 
				distance(other.distance), 
				entityA(other.entityA), 
				entityB(other.entityB)
			{}

			void operator=(const CollisionInfo& other)
			{
				containmentType = other.containmentType;
				contacts		= other.contacts;
				normal			= other.normal;
				colliderA		= other.colliderA;
				colliderB		= other.colliderB;
				distance		= other.distance;
				entityA			= other.entityA;
				entityB			= other.entityB;
			}

			CollisionInfo(const CollisionInfo&& other) noexcept :
				containmentType(std::move(other.containmentType)),
				contacts(std::move(other.contacts)), 
				normal(std::move(other.normal)),
				colliderA(std::move(other.colliderA)), 
				colliderB(std::move(other.colliderB)), 
				distance(std::move(other.distance)), 
				entityA(std::move(other.entityA)), 
				entityB(std::move(other.entityB))
			{}
		};
	}
}