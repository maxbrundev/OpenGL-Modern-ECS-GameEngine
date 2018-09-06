#pragma once

#include <ECS/Entity/IEntity.h>

namespace ECS
{
	template<typename T>
	class Entity : public IEntity
	{
	public:
		static const EntityTypeID ENTITY_TYPE_ID;

		virtual EntityTypeID GetEntityTypeID() const override { return ENTITY_TYPE_ID; }
		static EntityTypeID EntityTypeID() { return ENTITY_TYPE_ID; }

		Entity(const EntityID& p_id, ComponentManager* p_componentManager) : 
			IEntity(p_id, p_componentManager) {}

		virtual ~Entity() {}
	};

	template<typename T>
	const EntityTypeID Entity<T>::ENTITY_TYPE_ID = Utils::FamilyTypeID<IEntity>::Get<T>();
}