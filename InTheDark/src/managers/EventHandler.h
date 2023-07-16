#pragma once

#include "CanvasManager.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../util/util.h"

struct InternalMouseHandler
{
	bool pressed(int button);
	bool released(int button);
	glm::vec2 getPosition();
	double getOffset();
	void setOffset(double);

	void enableScrollCallback();

private:
	static double offset;
	CanvasManager& canvas = CanvasManager::getInstance();
	
	void enableScrollCallback(void (*callback)(GLFWwindow*, double, double));
};

struct InternalKeyHandler
{
	bool pressed(int key);
	bool released(int key);

private:
	CanvasManager& canvas = CanvasManager::getInstance();
};

class EventHandler
{
public:
	InternalMouseHandler mouse;
	InternalKeyHandler key;

	static EventHandler& getInstance()
	{
		static EventHandler instance;
		return instance;
	};

	void poll();
	bool gameShouldEnd();

	EventHandler(EventHandler const&) = delete;
	void operator=(EventHandler const&) = delete;

private:
	EventHandler() { };

	CanvasManager& canvas = CanvasManager::getInstance();
};