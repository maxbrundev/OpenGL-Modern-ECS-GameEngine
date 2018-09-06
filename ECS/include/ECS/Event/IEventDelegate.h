#pragma once

#include "Event.h"

namespace ECS
{
	namespace Event
	{
		class IEvent;
		using EventDelegateID = size_t;

		class IEventDelegate
		{
		public:
			virtual ~IEventDelegate() = default;

			virtual inline void Invoke(const IEvent* e) = 0;

			virtual inline EventDelegateID GetDelegateID() const = 0;

			virtual inline EventTypeID GetEventTypeID() const = 0;

			virtual bool operator==(const IEventDelegate* other) const = 0;

			virtual IEventDelegate* Clone() = 0;

		}; // class IEventDelegate
	}
}