#include "utils_stdafx.h"

#include <Utils/Timer.h>

using namespace Utils;

Timer::clock::time_point Timer::currentTime = clock::now();

long Timer::frameCount = 0;

Timer::FrameTime Timer::fixedDeltaTime(1/60.f);
Timer::FrameTime Timer::maxDeltaTime(1/60.f);

Timer::FrameTime Timer::elapsedTime(0.f);
Timer::FrameTime Timer::deltaTime(0.f);
Timer::FrameTime Timer::accumulator(0.f);

void Timer::PreUpdate()
{
	frameCount++;
	const auto newTime = clock::now();
	deltaTime = newTime - currentTime;
	if (deltaTime > maxDeltaTime)
		deltaTime = maxDeltaTime;

	currentTime = newTime;
	accumulator += deltaTime;
}

void Timer::FixedUpdate()
{
	elapsedTime += fixedDeltaTime;
	accumulator -= fixedDeltaTime;
}

Timer::FrameTime& Timer::FixedDeltaTime()
{
	return fixedDeltaTime;
}

Timer::FrameTime& Timer::DeltaTime()
{
	return deltaTime;
}

Timer::FrameTime& Timer::Accumulator()
{
	return accumulator;
}