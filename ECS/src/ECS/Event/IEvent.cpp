#include "ecs_stdafx.h"
#include "ECS/Event/IEvent.h"

using namespace ECS::Event;


IEvent::IEvent(const EventTypeID p_type_ID): m_type_ID(p_type_ID)
{
	ASSERT(ECS_Engine != nullptr, "ECS engine not initialized!");
	m_timeCreated = Utils::Timer::elapsedTime;
}
