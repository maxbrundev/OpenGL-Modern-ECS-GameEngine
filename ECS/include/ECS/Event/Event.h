#pragma once

#include <typeinfo>

#include "IEvent.h"

namespace ECS
{
	namespace Event
	{
		template <typename T>
		class Event : public IEvent
		{
		public:
			static const EventTypeID EVENT_TYPE_ID;
			static EventTypeID GetEventTypeID() { return EVENT_TYPE_ID; }

			Event() : IEvent(EVENT_TYPE_ID)
			{}
		};

		template<class T>
		const EventTypeID Event<T>::EVENT_TYPE_ID = typeid(T).hash_code();
	}
}