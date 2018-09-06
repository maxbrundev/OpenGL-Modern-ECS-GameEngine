#include "ecs_stdafx.h"
#include "ECS/Event/EventHandler.h"

using namespace ECS::Event;

EventHandler::EventHandler()
{
	this->m_eventStorage.reserve(1024);
}


EventHandler::~EventHandler()
{
	for (EventDispatcherMap::iterator it = this->m_eventDispatcherMap.begin(); it != this->m_eventDispatcherMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = nullptr;
	}

	this->m_eventDispatcherMap.clear();

	this->ClearEventBuffer();
}
