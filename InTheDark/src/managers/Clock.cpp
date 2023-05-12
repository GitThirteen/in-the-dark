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
	this->cur_time = std::chrono::steady_clock::now();
}

void Clock::tock()
{
	if (first_frame)
	{
		this->dt = this->cur_time - this->cur_time;
		this->first_frame = false;
		return;
	}

	this->dt = this->cur_time - this->last_time;
	this->last_time = this->cur_time;
}

void Clock::sleep(int millis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}