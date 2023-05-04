#include "CanvasManager.h"

void CanvasManager::clear()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void CanvasManager::clear(uint8_t r, uint8_t g, uint8_t b)
{
	clear(r, g, b, 255);
}

void CanvasManager::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	float rgb_r = r / 255.0f;
	float rgb_g = g / 255.0f;
	float rgb_b = b / 255.0f;
	float rgb_a = alpha / 255.0f;
	glClearColor(rgb_r, rgb_g, rgb_b, alpha);
}