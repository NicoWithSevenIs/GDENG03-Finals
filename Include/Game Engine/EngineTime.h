#pragma once

#include <chrono>
#include <ctime>

#include "Interfaces/Singleton.h"

class EngineTime : Singleton<EngineTime>
{
private:
	double delta_time = 0.0;

private:
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;

public:
	static double deltaTime();

public:
	static void LogFrameStart();
	static void LogFrameEnd();

};

