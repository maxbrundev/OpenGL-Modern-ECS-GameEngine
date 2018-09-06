#include "ecs_stdafx.h"
#include <ECS/Managers/EntityManager.h>

using namespace ECS;
EntityManager::EntityManager(ComponentManager* p_componentManager): m_componentManager(p_componentManager)
{
}

EntityManager::~EntityManager()
{
}

void EntityManager::DestroyEntity(const EntityID& p_entityID)
{
	const EntityTypeID EID = m_entityTable[p_entityID]->GetEntityTypeID();

	m_componentManager->RemoveAllComponents(p_entityID);

	m_entityTypedTables[EID]->DestroyEntity(p_entityID);
	m_entityTable.erase(p_entityID);
}

IEntity* EntityManager::GetEntity(const EntityID& p_entityID)
{
	return this->m_entityTable[p_entityID].get();
}
