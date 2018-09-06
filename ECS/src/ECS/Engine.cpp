#include "ecs_stdafx.h"
#include <ECS/Engine.h>

//#include "ECS/Event/EventHandler.h"

#include <ECS/Managers/EntityManager.h>
#include <ECS/Managers/ComponentManager.h>
#include <ECS/Managers/SystemManager.h>

namespace ECS
{
	ECSEngine::ECSEngine() : 
		ECS_ComponentManager(new ComponentManager()),
		ECS_SystemManager	(new SystemManager()),
		ECS_EventHandler	(new Event::EventHandler())
	{
		ECS_EntityManager = new EntityManager(this->ECS_ComponentManager);
	}

	ECSEngine::~ECSEngine()
	{
		delete ECS_EntityManager;
		ECS_EntityManager = nullptr;

		delete ECS_ComponentManager;
		ECS_ComponentManager = nullptr;

		delete ECS_SystemManager;
		ECS_SystemManager = nullptr;

		delete ECS_EventHandler;
		ECS_EventHandler = nullptr;
	}

	void ECSEngine::FixedUpdate(const double fixed_dt_ms) const
	{
		ECS_SystemManager->FixedUpdate(fixed_dt_ms);
		ECS_EventHandler->DispatchEvents();
	}

	void ECSEngine::Update(const double tick_ms) const
	{
		ECS_SystemManager->Update(tick_ms);
		ECS_EventHandler->DispatchEvents();
	}


	EntityManager*		ECSEngine::GetEntityManager() const		{ return ECS_EntityManager;}
	ComponentManager*	ECSEngine::GetComponentManager() const	{ return ECS_ComponentManager; }
	SystemManager*		ECSEngine::GetSystemManager() const		{ return ECS_SystemManager; }
}
