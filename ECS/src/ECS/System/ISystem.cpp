#include "ecs_stdafx.h"
#include "ECS/System/ISystem.h"

using namespace ECS;

ISystem::ISystem(const SystemPriority p_priority, const double p_updateInterval_ms) :
	m_priority(p_priority),
	m_updateInterval(p_updateInterval_ms),
	m_enabled(true),
	m_needsUpdate(true),
	m_reserved(0)
{
}

ISystem::~ISystem()
{}
