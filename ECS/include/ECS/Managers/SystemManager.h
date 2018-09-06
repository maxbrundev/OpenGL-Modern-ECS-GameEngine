#pragma once

#include "ecs_stdafx.h"
#include <ECS/ECS_API.h>

#include <ECS/Engine.h>
#include <ECS/System/ISystem.h>

#include "Debug/Log.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace ECS
{
	class ECS_API SystemManager
	{
		friend ECSEngine;

	private:
		using ISystem_ptr = std::shared_ptr<ISystem>;

		std::unordered_map<SystemTypeID, ISystem_ptr> m_systemTable;

		// System Dependency Table
		//    ---   |	Dependency SystemID |
		// SystemID |   0   |   1   |   2   |
		//     0    | true  | false | false |
		//     1    | false | false | false |
		//     2    | false | true  | false |

		std::vector<std::vector<bool>> m_systemDependencyTable;

		std::vector<ISystem_ptr> m_systemWorkOrder;

	public:
		SystemManager();
		~SystemManager();

		template<class T, class... ARGS>
		T* AddSystem(ARGS&&... systemArgs)
		{
			const SystemTypeID STID = T::SystemTypeID();

			// avoid multiple registrations of the same system
			auto it = this->m_systemTable.find(STID);
			if ((this->m_systemTable.find(STID) != this->m_systemTable.end()) && (it->second != nullptr))
				return reinterpret_cast<T*>(it->second.get());

			auto system = std::make_shared<T>(std::forward<ARGS>(systemArgs)...);
			system->m_systemManager = this;
			this->m_systemTable[STID] = system;

			// resize dependency table
			if (STID + 1 > this->m_systemDependencyTable.size())
			{
				this->m_systemDependencyTable.resize(STID + 1);
				for (int i = 0; i < this->m_systemDependencyTable.size(); ++i)
					this->m_systemDependencyTable[i].resize(STID + 1);
			}

			// add to work list
			this->m_systemWorkOrder.push_back(system);

			return system.get();
		}

		template<class System_, class Dependency_>
		void AddSystemDependency(System_ target, Dependency_ dependency)
		{
			const TypeID TARGET_ID = target->GetSystemTypeID();
			const TypeID DEPEND_ID = dependency->GetSystemTypeID();

			if (this->m_systemDependencyTable[TARGET_ID][DEPEND_ID] != true)
			{
				this->m_systemDependencyTable[TARGET_ID][DEPEND_ID] = true;
				LOG(Info(),"added " << dependency->GetSystemTypeName() << " as dependency to " << target->GetSystemTypeName());
			}
		}

		template<class Target_, class Dependency_, class... Dependencies>
		void AddSystemDependency(Target_ target, Dependency_ dependency, Dependencies&&... dependencies)
		{
			const TypeID TARGET_ID = target->GetSystemTypeID();
			const TypeID DEPEND_ID = dependency->GetSystemTypeID();

			if (this->m_systemDependencyTable[TARGET_ID][DEPEND_ID] != true)
			{
				this->m_systemDependencyTable[TARGET_ID][DEPEND_ID] = true;
				LOG(Info(), "added " << dependency->GetSystemTypeName() << " as dependency to " << target->GetSystemTypeName());
			}

			this->AddSystemDependency(target, std::forward<Dependencies>(dependencies)...);

//			this->UpdateSystemWorkOrder();
		}

		void UpdateSystemWorkOrder();

		template<class T>
		inline T* GetSystem() const
		{
			auto it = this->m_systemTable.find(T::SystemTypeID());

			return it != this->m_systemTable.end() ? (T*)it->second.get() : nullptr;
		}

		template<class T>
		void EnableSystem()
		{
			const SystemTypeID STID = T::SystemTypeID();

			// get system
			auto it = this->m_systemTable.find(STID);
			if (it != this->m_systemTable.end())
			{
				if (it->second->m_enabled == true)
					return;

				it->second->m_enabled = true;
			}
			else
			{
//				throw new EXCEPTION_DEBUG_INFO();
				LOG(Warning(), "Trying to enable system " << STID << ", but system is not registered yet.");
			}
		}

		template<class T>
		void DisableSystem()
		{
			const SystemTypeID STID = T::SystemTypeID();

			// get system
			auto it = this->m_systemTable.find(STID);
			if (it != this->m_systemTable.end())
			{
				if (it->second->m_enabled == false)
					return;

				it->second->m_enabled = false;
			}
			else
			{
//				throw new EXCEPTION_DEBUG_INFO();
				LOG(Warning(), "Trying to disable system " << STID << ", but system is not registered yet.");
			}
		}

		template<class T>
		void SetSystemUpdateInterval(double interval_ms)
		{
			const SystemTypeID STID = T::SystemTypeID();

			// get system
			auto it = this->m_systemTable.find(STID);
			if (it != this->m_systemTable.end())
			{
				it->second->m_updateInterval = interval_ms;
			}
			else
			{
//				throw new EXCEPTION_DEBUG_INFO();
				LOG(Warning(), "Trying to change system's " << STID << " update interval, but system is not registered yet.");
			}
		}

	private:
		void FixedUpdate(double fixed_dt_ms);
		void Update(double dt_ms);
	};
}

#pragma warning(pop)