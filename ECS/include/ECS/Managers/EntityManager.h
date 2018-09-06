#pragma once

#include "ecs_stdafx.h"
#include "ECS/ECS_API.h"

#include "ECS/Entity/Entity.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace ECS
{
	using IEntity_ptr = std::shared_ptr<IEntity>;

	class ECS_API EntityManager
	{
	private: 
		// IEntityContainer
		class IEntityContainer
		{
		public:

			virtual ~IEntityContainer()
			{}

//			virtual const char* GetEntityContainerTypeName() const = 0;
			virtual void DestroyEntity(const EntityID& p_entityID) = 0;
		}; // IEntityContainer

		// EntityContainer<T>
		template<typename T>
		class EntityContainer : public IEntityContainer
		{
			using TypedEntityContainer = std::unordered_map<EntityID, std::shared_ptr<Entity<T>>>;

		private:
			TypedEntityContainer m_typedContainer;

		public:
			EntityContainer() {}
			TypedEntityContainer& GetTypedEntityContainer() { return m_typedContainer; }
			void DestroyEntity(const EntityID& p_entityID) override
			{
				this->m_typedContainer.erase(p_entityID);
			}
		}; // EntityContainer<T>

	private:
		using IEntityContainer_ptr = std::shared_ptr<IEntityContainer>;

		std::unordered_map<EntityTypeID, IEntityContainer_ptr>	m_entityTypedTables;
		std::unordered_map<EntityID, IEntity_ptr>				m_entityTable;
		ComponentManager*										m_componentManager;

	public:
		explicit EntityManager(ComponentManager* p_componentManager); 
		~EntityManager();
		
		template<typename T, typename ...Args>
		T& CreateEntity(Args&&... p_params)
		{
			EntityID newID = Utils::FamilyObjectID<IEntity>::Get();
			auto newEntity = std::make_shared<T>(newID, this->m_componentManager, std::forward<Args>(p_params)...);
			auto result = m_entityTable.try_emplace(newID, newEntity);
			ASSERT(result.second, "Error creating new Entity !");

			auto result2 = GetEntityContainer<T>()->GetTypedEntityContainer().try_emplace(newID, newEntity);
			ASSERT(result2.second, "Error inserting Entity in typed containers !");

			return *newEntity.get();
		}

		void DestroyEntity(const EntityID& p_entityID);

		IEntity* GetEntity(const EntityID& p_entityID);

	private:

		template<typename T>
		EntityContainer<T>* GetEntityContainer()
		{
			try
			{
				EntityTypeID EID = T::EntityTypeID();

				auto result = this->m_entityTypedTables.find(EID);
				if (result != m_entityTypedTables.end())
					return reinterpret_cast<EntityContainer<T>*>(result->second.get());

				auto result2 = this->m_entityTypedTables.try_emplace(EID,std::make_unique<EntityContainer<T>>());
				ASSERT(result2.second, "Failed to create EntityContainer<T>!");
				return reinterpret_cast<EntityContainer<T>*>(result2.first->second.get());
			}
			catch (Debug::Assertion e)
			{
				e.LogError();
				return nullptr;
			}
		}
	};
}

#pragma warning(pop)