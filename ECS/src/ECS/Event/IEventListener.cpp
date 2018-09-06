#include "ecs_stdafx.h"
#include "ECS/Event/IEventListener.h"
#include "ECS/Engine.h"

using namespace ECS::Event;

IEventListener::IEventListener()
{}


IEventListener::~IEventListener()
{
	UnregisterAllEventCallbacks();
}

void IEventListener::UnregisterAllEventCallbacks()
{
	for (auto callback : this->m_registeredCallbacks)
	{
		ECS_Engine->UnsubscribeEvent(callback);
		delete callback;
	}

	this->m_registeredCallbacks.clear();
}
