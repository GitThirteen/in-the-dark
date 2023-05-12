#pragma once

#include <chrono>
#include <thread>

class Clock
{
public:
	/**
	 * @return The time passed between the last frame and the current one in fractions of a second (e.g., 0.0015).
	*/
	double getDeltaTime();

	/**
	 * @return The FPS as int based on the delta time.
	*/
	int getFPS();

	/**
	 * @brief Records the current time and calculates the delta time based on this value and the value from the last frame (or the last time tick() was called).
	*/
	void tick();

	/**
	 * @brief Sleeps for a set number of milliseconds. This function might take slightly longer than set and does not work well for very low values.
	 * @param The number of milliseconds to (at least) sleep for
	*/
	void sleep(int);

	static Clock& getInstance()
	{
		static Clock instance;
		return instance;
	}

	Clock(Clock const&) = delete;
	void operator=(Clock const&) = delete;
private:
	std::chrono::steady_clock::time_point frame_time;
	std::chrono::duration<double> dt;

	Clock() {
		this->frame_time = std::chrono::steady_clock::now();
	};
};