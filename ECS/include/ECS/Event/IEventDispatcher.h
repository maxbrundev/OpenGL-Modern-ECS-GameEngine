#pragma once

#include "ecs_stdafx.h"

#include "ECS/Event/IEventDelegate.h"

namespace ECS
{
	namespace Event
	{
		class IEvent;

		class IEventDispatcher
		{
		public:
			virtual ~IEventDispatcher() = default;

			virtual void Dispatch(const IEvent* event) = 0;

			virtual void AddEventCallback(IEventDelegate* const eventDelegate) = 0;

			virtual void RemoveEventCallback(IEventDelegate* eventDelegate) = 0;

			virtual inline size_t GetEventCallbackCount() const = 0;
		};
	}
}