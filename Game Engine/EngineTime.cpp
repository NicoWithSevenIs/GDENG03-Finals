#include "Game Engine/EngineTime.h"


double EngineTime::deltaTime()
{
	return EngineTime::get().delta_time;
}

void EngineTime::LogFrameStart()
{
	EngineTime::get().start = std::chrono::system_clock::now();
}

void EngineTime::LogFrameEnd()
{
	EngineTime::get().end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = EngineTime::get().end - EngineTime::get().start;

	EngineTime::get().delta_time = elapsed.count();
}
