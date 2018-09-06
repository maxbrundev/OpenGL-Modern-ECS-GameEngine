#include "ecs_stdafx.h"
#include <ECS/Utils/GUID.h>

namespace ECS
{
	class IEntity;
	class IComponent;
	class ISystem;
}

namespace Utils
{

	TypeID FamilyTypeID<ECS::IEntity>::s_count		= 0u;
	TypeID FamilyTypeID<ECS::IComponent>::s_count	= 0u;
	TypeID FamilyTypeID<ECS::ISystem>::s_count		= 0u;

	template class FamilyTypeID<ECS::IEntity>;
	template class FamilyTypeID<ECS::IComponent>;
	template class FamilyTypeID<ECS::ISystem>;
	
	TypeID FamilyObjectID<ECS::IEntity>::s_count	= 0u;
	TypeID FamilyObjectID<ECS::IComponent>::s_count = 0u;
	TypeID FamilyObjectID<ECS::ISystem>::s_count	= 0u;

	template class FamilyObjectID<ECS::IEntity>;
	template class FamilyObjectID<ECS::IComponent>;
	template class FamilyObjectID<ECS::ISystem>;
}