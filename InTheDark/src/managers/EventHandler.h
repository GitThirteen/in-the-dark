#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

typedef struct InternalMouseHandler
{
	InternalMouseHandler(GLFWwindow* window);

	bool pressed(int button);
	bool released(int button);
	glm::vec2 getPosition();

private:
	GLFWwindow* window;
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
	void enableScrollCallback();

	float getCurrentRadius();

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