#pragma once

#include <ECS/ECS_API.h>
#include <ECS/Types.h>
#include <ECS/Managers/ComponentManager.h>

namespace ECS
{
	class ECS_API IEntity
	{
		friend class EntityManager;

	private:
		ComponentManager* m_componentManager;

	protected:
		EntityID m_entityID;
		bool m_active;

	public:
		IEntity(const EntityID& p_id, ComponentManager* p_componentManager) : m_componentManager(p_componentManager), m_entityID(p_id), m_active(true)
		{}

		virtual ~IEntity() = default;

		template<typename T>
		T* GetComponent() const
		{
			return this->m_componentManager->GetComponent<T>(this->m_entityID);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&&... p_params)
		{
			return this->m_componentManager->AddComponent<T>(this->m_entityID, std::forward<Args>(p_params)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			return this->m_componentManager->RemoveComponent<T>(this->m_entityID);
		}

		void SetActive(const bool p_active)
		{
			if (this->m_active == p_active)
				return;

			if (p_active == false)
				this->OnDisable();
			else
				this->OnEnable();

			this->m_active = p_active;
		}

		inline const EntityID& GetEntityID() const { return this->m_entityID; }
		virtual EntityTypeID GetEntityTypeID() const = 0;

		inline bool IsActive() const { return this->m_active; }

		virtual void OnEnable() {}
		virtual void OnDisable() {}

		inline bool operator==(const IEntity& p_other) const { return this->m_entityID == p_other.m_entityID; }
		inline bool operator!=(const IEntity& p_other) const { return this->m_entityID != p_other.m_entityID; }
		inline bool operator==(const IEntity* p_other) const { return this->m_entityID == p_other->m_entityID; }
		inline bool operator!=(const IEntity* p_other) const { return this->m_entityID != p_other->m_entityID; }
	};
}