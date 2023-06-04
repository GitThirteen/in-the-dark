#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../util/util.h"

struct InternalMouseHandler
{
	InternalMouseHandler(GLFWwindow* window);

	bool pressed(int button);
	bool released(int button);
	glm::vec2 getPosition();
	double getOffset();
	void setOffset(double);

	void enableScrollCallback();

private:
	GLFWwindow* window;
	static double offset;
	
	void enableScrollCallback(void (*callback)(GLFWwindow*, double, double));
};

struct InternalKeyHandler
{
	InternalKeyHandler(GLFWwindow* window);

	bool pressed(int key);
	bool released(int key);

private:
	GLFWwindow* window;
};

class EventHandler
{
public:
	InternalMouseHandler mouse = nullptr;
	InternalKeyHandler key = nullptr;

	static EventHandler& getInstance()
	{
		static EventHandler instance;
		return instance;
	};

	void setWindow(GLFWwindow* window)
	{
		this->window = window;
		this->mouse = InternalMouseHandler(window);
		this->key = InternalKeyHandler(window);
	};

	void poll();

	EventHandler(EventHandler const&) = delete;
	void operator=(EventHandler const&) = delete;

protected:
	GLFWwindow* window;

private:
	EventHandler() { };
};