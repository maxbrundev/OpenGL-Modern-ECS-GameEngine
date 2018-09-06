#pragma once

#include "ECS_API.h"

#include "Event/EventHandler.h"
#include "Event/IEventListener.h"

namespace ECS
{
	namespace Util
	{
		class Timer;
	}

	class EntityManager;
	class SystemManager;
	class ComponentManager;

	class ECS_API ECSEngine
	{
		friend class IEntity;
		friend class IComponent;
		friend class ISystem;

		friend class Event::IEvent;
		friend class Event::IEventListener;

	private:
		EntityManager*			ECS_EntityManager;
		ComponentManager*		ECS_ComponentManager;
		SystemManager*			ECS_SystemManager;
		Event::EventHandler*	ECS_EventHandler;

		ECSEngine(const ECSEngine&) = delete;
		ECSEngine& operator=(ECSEngine&) = delete;

		template<class E>
		inline void SubscribeEvent(Event::IEventDelegate* const eventDelegate)
		{
			ECS_EventHandler->AddEventCallback<E>(eventDelegate);
		}

		inline void UnsubscribeEvent(Event::IEventDelegate* eventDelegate) const
		{
			ECS_EventHandler->RemoveEventCallback(eventDelegate);
		}

	public:
		ECSEngine();
		~ECSEngine();

		template<class E, class... Args>
		void SendEvent(Args&&... eventArgs)
		{
			ECS_EventHandler->Send<E>(std::forward<Args>(eventArgs)...);
		}

		void FixedUpdate(double fixed_dt_ms) const;
		void Update(double tick_ms) const;

		EntityManager* GetEntityManager() const;
		ComponentManager* GetComponentManager() const;
		SystemManager* GetSystemManager() const;
	};
}