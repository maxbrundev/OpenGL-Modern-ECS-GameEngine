#pragma once

#include "ECS/ECS_API.h"

#include "EventDelegate.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace ECS
{
	namespace Event
	{
		class ECS_API IEventListener
		{
		private:
			std::list<IEventDelegate*> m_registeredCallbacks;

		public:
			IEventListener();
			virtual ~IEventListener();

			template<typename Listener, typename EventType>
			inline void RegisterEventCallback(void(Listener::*p_callback)(const EventType*))
			{
				IEventDelegate* eventDelegate = new EventDelegate<Listener, EventType>(static_cast<Listener*>(this), p_callback);

				m_registeredCallbacks.push_back(eventDelegate);
				ECS_Engine->SubscribeEvent<EventType>(eventDelegate);
			}

			template<typename Listener, typename EventType>
			inline void UnregisterEventCallback(void(Listener::*p_callback)(const EventType*))
			{
				EventDelegate<Listener, EventType> eDelegate(static_cast<Listener*>(this), p_callback);

				for (auto callback : this->m_registeredCallbacks)
				{
					if (callback->GetDelegateID() == eDelegate.GetDelegateID())
					{
						auto it = std::find_if(this->m_registeredCallbacks.begin(), this->m_registeredCallbacks.end(), [&](const IEventDelegate* other)
						{
							return (*other) == callback;
						});
						
						ECS_Engine->UnsubscribeEvent(&eDelegate);

						if (it != this->m_registeredCallbacks.end())
						{
							IEventDelegate* delegate = *it;
							this->m_registeredCallbacks.remove(delegate);
							delete delegate;
						}
						break;
					}
				}
			}
			void UnregisterAllEventCallbacks();
		};
	}
}

#pragma warning(pop)