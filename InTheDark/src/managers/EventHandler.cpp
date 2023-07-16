#include "EventHandler.h"

void EventHandler::poll()
{
	glfwPollEvents();
}

bool EventHandler::gameShouldEnd()
{
	return glfwWindowShouldClose(canvas.window.get());
}

// Mouse Handler

double InternalMouseHandler::offset = 20;

bool InternalMouseHandler::pressed(int button)
{
	return glfwGetMouseButton(canvas.window.get(), button) == GLFW_PRESS;
}

bool InternalMouseHandler::released(int button)
{
	return glfwGetMouseButton(canvas.window.get(), button) == GLFW_RELEASE;
}

glm::vec2 InternalMouseHandler::getPosition()
{
	double m_x = 0.0, m_y = 0.0;
	glfwGetCursorPos(canvas.window.get(), &m_x, &m_y);
	return glm::vec2(m_x, m_y);
}

double InternalMouseHandler::getOffset()
{
	return offset;
}

void InternalMouseHandler::setOffset(double offset)
{
	this->offset = offset;
}

void InternalMouseHandler::enableScrollCallback()
{
	enableScrollCallback([](GLFWwindow*, double, double y_offset) -> void {
		double dy = offset - (y_offset * 0.75);
		offset = util::clamp(dy, 5, 40);
	});
}

void InternalMouseHandler::enableScrollCallback(void (*callback)(GLFWwindow*, double, double))
{
	glfwSetScrollCallback(canvas.window.get(), callback);
}

// Key Handler

bool InternalKeyHandler::pressed(int key)
{
	return glfwGetKey(canvas.window.get(), key) == GLFW_PRESS;
}

bool InternalKeyHandler::released(int key)
{
	return glfwGetKey(canvas.window.get(), key) == GLFW_RELEASE;
}