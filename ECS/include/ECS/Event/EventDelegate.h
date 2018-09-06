#pragma once

#include "ecs_stdafx.h"

#include "IEventDelegate.h"

namespace ECS
{
	namespace Event
	{
		class IEvent;

		using EventDelegateID = size_t;
		
		template<typename Listener, typename EventType>
		class EventDelegate : public IEventDelegate
		{
			typedef void(Listener::*Callback)(const EventType*);

			Listener* m_receiver;
			Callback m_callback;

		public:
			EventDelegate(Listener* p_receiver, Callback& p_callbackFunction) : 
				m_receiver(p_receiver), 
				m_callback(p_callbackFunction)
			{}

			virtual IEventDelegate* Clone() override
			{
				return new EventDelegate(this->m_receiver, this->m_callback);
			}

			virtual inline void Invoke(const IEvent* p_event) override
			{
				(m_receiver->*m_callback)(reinterpret_cast<const EventType*>(p_event));
			}

			virtual inline EventDelegateID GetDelegateID() const override
			{
				static const EventDelegateID delegate_ID{ typeid(Listener).hash_code() ^ typeid(Callback).hash_code() };
				return delegate_ID;
			}

			virtual inline EventTypeID GetEventTypeID() const override
			{
				static const EventTypeID ETID{ EventType::GetEventTypeID() };
				return ETID;
			}

			virtual bool operator==(const IEventDelegate* other) const override
			{
				if (other == nullptr)
					return false;

				if (this->GetDelegateID() != other->GetDelegateID())
					return false;

				EventDelegate* delegate = (EventDelegate*)(other);
				return ((this->m_callback == delegate->m_callback) && (this->m_receiver == delegate->m_receiver));
			}
		}; // class IEventDelegate
	}
}