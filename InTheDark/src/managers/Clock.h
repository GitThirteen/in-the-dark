#pragma once

#include <chrono>

class Clock
{
public:
	double getDeltaTime();
	int getFPS();

	static Clock& getInstance()
	{
		static Clock instance;
		return instance;
	}

	void tick();
	void tock();

	Clock(Clock const&) = delete;
	void operator=(Clock const&) = delete;
private:
	std::chrono::steady_clock::time_point cur_time;
	std::chrono::steady_clock::time_point last_time;
	std::chrono::duration<double> dt;

	bool first_frame = true;

	Clock() { };
};