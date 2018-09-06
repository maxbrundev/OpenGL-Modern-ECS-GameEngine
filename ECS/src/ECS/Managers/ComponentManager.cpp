#include "ecs_stdafx.h"

#include "ECS/Managers/ComponentManager.h"

using namespace ECS;

void ComponentManager::RemoveAllComponents(const size_t p_entityID)
{
	if(m_entityComponentTable.find(p_entityID) != m_entityComponentTable.end())
	{
		auto components = m_entityComponentTable.at(p_entityID);
		for (auto i = components.begin(), end = components.end(); i != end;) {
			m_componentTable.erase(i->second);
			m_componentTypedTables[i->first]->DestroyComponent(p_entityID);
			i = components.erase(i);
		}
	}
}