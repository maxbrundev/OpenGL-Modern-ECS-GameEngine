#pragma once

#include "utils_stdafx.h"
#include "UtilsAPI.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace Utils
{
	class UTILS_API Timer
	{
	private:
		Timer() = delete;
		~Timer() = delete;
		
	public:
		using clock = std::chrono::high_resolution_clock;
		using ms = std::chrono::duration<double, std::milli>;
		using sec = std::chrono::duration<double>;

		using FrameTime = sec;

		static long frameCount;

		static FrameTime fixedDeltaTime;
		static FrameTime maxDeltaTime;

		static clock::time_point currentTime;
		static FrameTime elapsedTime;
		static FrameTime deltaTime;
		static FrameTime accumulator;

		static void PreUpdate();
		static void FixedUpdate();

		static FrameTime&	FixedDeltaTime();
		static FrameTime&	DeltaTime();
		static FrameTime&	Accumulator();
	};
}

#pragma warning (pop)