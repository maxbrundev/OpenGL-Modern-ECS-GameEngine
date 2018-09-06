#pragma once

#include "ecs_stdafx.h"
#include "ECS/ECS_API.h"

#include "ECS/Types.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace ECS
{
	namespace Event
	{
		using EventTypeID = TypeID;
		using EventTimestamp = Utils::Timer::FrameTime;

		class ECS_API IEvent
		{
		private:
			EventTypeID		m_type_ID;
			EventTimestamp	m_timeCreated;

		public:
			explicit IEvent(const EventTypeID p_type_ID);

			EventTypeID	 GetEventTypeID()	const { return this->m_type_ID; }
			EventTimestamp GetTimeCreated()	const { return this->m_timeCreated; }

		};
	}
}

#pragma warning(pop)