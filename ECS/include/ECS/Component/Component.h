#pragma once

#include <ECS/Component/IComponent.h>
#include <ECS/Utils/GUID.h>

#define COMPONENT_INIT_PARAMS const ECS::EntityID& p_ownerID, const ECS::ComponentID& p_componentID
#define COMPONENT_INIT(Type) Component<Type>(p_ownerID, p_componentID)

namespace ECS
{
	template<typename T>
	class Component : public IComponent
	{
	public:
		static const ComponentTypeID COMPONENT_TYPE_ID;

		ComponentTypeID GetComponentTypeID() const override { return COMPONENT_TYPE_ID; }
		static ComponentTypeID ComponentTypeID() { return COMPONENT_TYPE_ID; }

		Component(COMPONENT_INIT_PARAMS): IComponent(p_ownerID,p_componentID) {}

		virtual ~Component() {}
	};

	template<typename T>
	const ComponentTypeID Component<T>::COMPONENT_TYPE_ID = Utils::FamilyTypeID<IComponent>::Get<T>();
}