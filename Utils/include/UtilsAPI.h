#pragma once

#ifdef UTILS_EXPORT
#define UTILS_API __declspec(dllexport)   
#else
#define UTILS_API __declspec(dllimport)   
#endif 