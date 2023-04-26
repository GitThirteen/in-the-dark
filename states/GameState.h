#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "../managers/AssetManager.h"

class GameState
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void discard() = 0;

	virtual ~GameState() { };

protected:
	AssetManager& assets = AssetManager::getInstance();
};
