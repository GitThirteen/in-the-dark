#include "Clock.h"

double Clock::getDeltaTime()
{
	return this->dt.count();
}

int Clock::getDeltaTimeAsMillis()
{
	auto cast_dt = std::chrono::duration_cast<std::chrono::milliseconds>(this->dt);
	return cast_dt.count();
}

int Clock::getFPS()
{
	int fps = 1 / this->dt.count();
	return std::max(0, fps);
}

void Clock::tick()
{
	auto cur_time = std::chrono::steady_clock::now();
	this->dt = cur_time - this->frame_time;
	this->frame_time = cur_time;
}

void Clock::sleep(int millis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}