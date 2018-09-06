#pragma once

#ifdef RENDERER_EXPORT
#define RENDERER_API __declspec(dllexport)   
#else
#define RENDERER_API __declspec(dllimport)   
#endif 