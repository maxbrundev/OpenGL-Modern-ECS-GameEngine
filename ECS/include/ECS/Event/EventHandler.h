#pragma once

#include "IEvent.h"
#include "IEventDispatcher.h"
#include "EventDispatcher.h"

namespace ECS 
{
	namespace Event 
	{
		class EventHandler
		{
		private:
			friend class ECSEngine;

			using EventDispatcherMap = std::unordered_map<EventTypeID, IEventDispatcher*>;

		private:
			EventHandler(const EventHandler&) = delete;
			EventHandler& operator=(EventHandler&) = delete;

			EventDispatcherMap	m_eventDispatcherMap;
			std::vector<IEvent*>	m_eventStorage;

			template<typename EventType>
			inline void AddEventCallback(IEventDelegate* const eventDelegate)
			{
				EventTypeID ETID = EventType::GetEventTypeID();

				const EventDispatcherMap::const_iterator it = this->m_eventDispatcherMap.find(ETID);
				if (it == this->m_eventDispatcherMap.end())
				{
					std::pair<EventTypeID, IEventDispatcher*> eventDispatcher = std::make_pair(ETID, new EventDispatcher<EventType>());
					eventDispatcher.second->AddEventCallback(eventDelegate);

					this->m_eventDispatcherMap.insert(eventDispatcher);
				}
				else
				{
					this->m_eventDispatcherMap[ETID]->AddEventCallback(eventDelegate);
				}
			}

			// Remove event callback
			inline void RemoveEventCallback(IEventDelegate* eventDelegate)
			{
				const EventTypeID ETID = eventDelegate->GetEventTypeID();
				const EventDispatcherMap::const_iterator it = this->m_eventDispatcherMap.find(ETID);
				if (it != this->m_eventDispatcherMap.end())
				{
					this->m_eventDispatcherMap[ETID]->RemoveEventCallback(eventDelegate);
				}
			}

		public:
			EventHandler();
			~EventHandler();

			inline void ClearEventBuffer()
			{
				for (IEvent* event : this->m_eventStorage)
				{
					delete event;
				}
				this->m_eventStorage.clear();
			}

			inline void ClearEventDispatcher()
			{
				this->m_eventDispatcherMap.clear();
			}

			template<class EventType, class... Args>
			void Send(Args&&... eventArgs)
			{
				// check if type of object is trivially copyable
				static_assert(std::is_trivially_copyable<EventType>::value, "Event is not trivially copyable.");

				this->m_eventStorage.push_back(new EventType(std::forward<Args>(eventArgs)...));
			}

			void DispatchEvents()
			{
				size_t lastIndex = this->m_eventStorage.size();
				size_t thisIndex = 0;

				while (thisIndex < lastIndex)
				{
					const auto event = this->m_eventStorage.at(thisIndex++);

					auto it = this->m_eventDispatcherMap.find(event->GetEventTypeID());
					if (it == this->m_eventDispatcherMap.end())
						continue;

					it->second->Dispatch(event);
					lastIndex = this->m_eventStorage.size();
				}
				ClearEventBuffer();
			}
		};
	}
}