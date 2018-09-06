#pragma once

#ifdef ECS_EXPORT
#define ECS_API __declspec(dllexport)   
#else
#define ECS_API __declspec(dllimport)   
#endif 

namespace ECS
{
	class ECSEngine;

	ECS_API extern ECSEngine*	ECS_Engine;

	ECS_API void				Initialize();
	ECS_API void				Terminate();
}