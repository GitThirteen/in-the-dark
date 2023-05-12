#include "Clock.h"

double Clock::getDeltaTime()
{
	return this->dt.count();
}

int Clock::getFPS()
{
	int fps = 1 / this->dt.count();
	return std::max(0, fps);
}

void Clock::tick()
{
	auto cur_time = std::chrono::steady_clock::now();
	this->dt = first_frame ? (cur_time - cur_time) : (cur_time - this->frame_time);
	this->frame_time = cur_time;

	if (first_frame) first_frame = false;
}

void Clock::sleep(int millis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}