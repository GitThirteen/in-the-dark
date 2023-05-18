#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../util/util.h"

struct InternalMouseHandler
{
	InternalMouseHandler(GLFWwindow* window);

	// Mouse-related getters
	bool pressed(int button);
	bool released(int button);
	glm::vec2 getPosition();
	double getOffset();

	// Misc.
	void enableScrollCallback();
private:
	GLFWwindow* window;
	static double offset;

	void enableScrollCallback(void (*callback)(GLFWwindow*, double, double));
};

class EventHandler
{
public:
	InternalMouseHandler mouse = nullptr;

	static EventHandler& getInstance()
	{
		static EventHandler instance;
		return instance;
	};

	void setWindow(GLFWwindow* window)
	{
		this->window = window;
		this->mouse = InternalMouseHandler(window);
	};

	void enableKeyCallback();

	void poll();

	EventHandler(EventHandler const&) = delete;
	void operator=(EventHandler const&) = delete;

protected:
	GLFWwindow* window;

private:
	EventHandler() { };

	void keyCallback();
	void scrollCallback();
};