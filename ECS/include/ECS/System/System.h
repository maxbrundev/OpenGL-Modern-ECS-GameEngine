#pragma once

#include "ecs_stdafx.h"

#include <ECS/System/ISystem.h>
#include <ECS/Utils/GUID.h>

namespace ECS
{
	template<class T>
	class System : public ISystem
	{
		friend class SystemManager;

	private:
		SystemManager* m_systemManager;

	public:
		static const SystemTypeID SYSTEM_TYPE_ID;

	protected:
		System(): m_systemManager(nullptr)
		{}

	public:
		virtual ~System()
		{}

		virtual inline SystemTypeID GetSystemTypeID() const override
		{
			return SYSTEM_TYPE_ID;
		}

		static SystemTypeID SystemTypeID()
		{
			return SYSTEM_TYPE_ID;
		}

		virtual inline const char* GetSystemTypeName() const override
		{
			static const char* SYSTEM_TYPE_NAME{ typeid(T).name() };
			return SYSTEM_TYPE_NAME;
		}

		template<class... Dependencies>
		void AddDependencies(Dependencies&&... dependencies)
		{
			this->m_systemManager->AddSystemDependency(this, std::forward<Dependencies>(dependencies)...);
		}

		virtual void FixedUpdate(double fixed_dt) override
		{}

		virtual void PreUpdate(double dt) override
		{}

		virtual void Update(double dt) override
		{}

		virtual void PostUpdate(double dt) override
		{}
	};

	template<typename T>
	const SystemTypeID System<T>::SYSTEM_TYPE_ID = Utils::FamilyTypeID<ISystem>::Get<T>();
}