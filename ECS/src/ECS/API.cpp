#include "ecs_stdafx.h"
#include "ECS/Engine.h"

namespace ECS
{
	ECSEngine*		ECS_Engine = nullptr;// new ECSEngine();

	void Initialize()
	{
		if (ECS_Engine == nullptr)
			ECS_Engine = new ECSEngine();
	}

	void Terminate()
	{
		if (ECS_Engine != nullptr)
		{
			delete ECS_Engine;
			ECS_Engine = nullptr;
		}
	}
}
