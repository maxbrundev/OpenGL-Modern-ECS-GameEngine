#pragma once

#include "ECS/ECS_API.h"

using ObjectID = size_t;
using TypeID = size_t;

namespace Utils
{
	template<typename T>
	class ECS_API FamilyTypeID
	{
	private:
		static TypeID s_count;

	public:
		template<typename U>
		static TypeID Get()
		{
			static const TypeID STATIC_TYPE_ID{ s_count++ };
			return STATIC_TYPE_ID;
		}

		static TypeID Get()
		{
			return s_count;
		}
	};

	template<typename T>
	class ECS_API FamilyObjectID
	{
	private:
		FamilyObjectID(){}
		static TypeID s_count;
	public:

		constexpr static TypeID Get()
		{
			return s_count++;
		}
	};
}