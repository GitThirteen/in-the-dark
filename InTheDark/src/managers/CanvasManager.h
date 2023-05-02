#pragma once
#include <cstdint>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

class CanvasManager
{
public:
	/**
	 * @brief Clears the canvas with white (RGB 255, 255, 255).
	*/
	void clear();
	/**
	 * @brief Clears the canvas with a custom RGB color code.
	 * @param r The red component of the color
	 * @param g The green component of the color
	 * @param b The blue component of the color
	*/
	void clear(uint8_t r, uint8_t g, uint8_t b);
	void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha);

	static CanvasManager& getInstance()
	{
		static CanvasManager instance;
		return instance;
	};

	CanvasManager(CanvasManager const&)		= delete;
	void operator=(CanvasManager const&)	= delete;

private:
	CanvasManager() { }
};