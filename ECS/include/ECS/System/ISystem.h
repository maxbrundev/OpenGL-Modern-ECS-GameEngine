#pragma once
#undef max
#undef min

#include "ecs_stdafx.h"
#include "ECS/ECS_API.h"

#include "ECS/Types.h"

namespace ECS
{
	template<class T>
	class System;

	using SystemPriority = uint16_t;

	static const SystemPriority LOWEST_SYSTEM_PRIORITY = std::numeric_limits<SystemPriority>::min();

	static const SystemPriority VERY_LOW_SYSTEM_PRIORITY = 99;
	static const SystemPriority LOW_SYSTEM_PRIORITY = 100;

	static const SystemPriority NORMAL_SYSTEM_PRIORITY = 200;

	static const SystemPriority MEDIUM_SYSTEM_PRIORITY = 300;

	static const SystemPriority HIGH_SYSTEM_PRIORITY = 400;
	static const SystemPriority VERY_HIGH_SYSTEM_PRIORITY = 401;

	static const SystemPriority HIGHEST_SYSTEM_PRIORITY = std::numeric_limits<SystemPriority>::max();

	class ECS_API ISystem
	{
		friend class SystemManager;

	private:
		double m_timeSinceLastUpdate{ 0 };

		SystemPriority m_priority;

		double m_updateInterval;

		uint8_t	m_enabled : 1;
		uint8_t	m_needsUpdate : 1;
		uint8_t	m_reserved : 6;

	protected:
		explicit ISystem(SystemPriority p_priority = NORMAL_SYSTEM_PRIORITY, double p_updateInterval_ms = (-1.0));

	public:
		virtual ~ISystem();

		virtual inline SystemTypeID GetSystemTypeID() const = 0;
		virtual inline const char* GetSystemTypeName() const = 0;

		virtual void FixedUpdate(double fixed_dt) = 0;
		virtual void PreUpdate(double dt) = 0;
		virtual void Update(double dt) = 0;
		virtual void PostUpdate(double dt) = 0;
	};
}