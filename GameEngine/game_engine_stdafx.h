// game_engine_stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <algorithm>
#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <stb_image.h>

#include <Utils.h>
#include <ECS.h>
#include <Core.h>
#include <Renderer.h>
#include <Physics.h>