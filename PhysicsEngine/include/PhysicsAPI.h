#pragma once

#ifdef PHYSICS_EXPORT
#define PHYSICS_API __declspec(dllexport)   
#else
#define PHYSICS_API __declspec(dllimport)   
#endif 