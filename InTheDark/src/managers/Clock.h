#pragma once

#include <chrono>
#include <thread>

class Clock
{
public:
	double getDeltaTime();
	int getFPS();

	void tick();
	void tock();
	void sleep(int);

	static Clock& getInstance()
	{
		static Clock instance;
		return instance;
	}

	Clock(Clock const&) = delete;
	void operator=(Clock const&) = delete;
private:
	std::chrono::steady_clock::time_point cur_time;
	std::chrono::steady_clock::time_point last_time;
	std::chrono::duration<double> dt;

	bool first_frame = true;

	Clock() { };
};