#pragma once
#include <cstdint>
#include <utility>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "SettingsManager.h"
#include "ShaderManager.h"
#include "../objects/GameAsset.h"

struct PostProcessor
{
void create();
void draw();
void bind();
void unbind();
void dump();
bool isCreated();

private:
	GLuint vao;
	GLuint fbo;
	GLuint colTex;
	GLuint depTex;
	GLuint shader;
	bool created = false;

	ShaderManager& shaders = ShaderManager::getInstance();
};

class CanvasManager
{
public:
	PostProcessor post_processor;

	/**
	 * @brief Clears the canvas with white (RGB 255, 255, 255).
	*/
	void clear();
	/**
	 * @brief Clears the canvas with a custom RGB color code.
	 * @param r The red component of the color (0-255)
	 * @param g The green component of the color (0-255)
	 * @param b The blue component of the color (0-255)
	*/
	void clear(uint8_t r, uint8_t g, uint8_t b);
	/**
	 * @brief Clears the canvas with a custom RGB color and alpha value.
	 * @param r The red component of the color (0-255)
	 * @param g The green component of the color (0-255)
	 * @param b The blue component of the color (0-255)
	 * @param alpha The transparency of the color (0-100)
	*/
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