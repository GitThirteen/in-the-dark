#include "EventHandler.h"

void EventHandler::poll()
{
	glfwPollEvents();
}

// Mouse Handler

double InternalMouseHandler::offset = 20;

InternalMouseHandler::InternalMouseHandler(GLFWwindow* window)
{
	this->window = window;
}

bool InternalMouseHandler::pressed(int button)
{
	return glfwGetMouseButton(this->window, button) == GLFW_PRESS;
}

bool InternalMouseHandler::released(int button)
{
	return glfwGetMouseButton(this->window, button) == GLFW_RELEASE;
}

glm::vec2 InternalMouseHandler::getPosition()
{
	double m_x = 0.0, m_y = 0.0;
	glfwGetCursorPos(this->window, &m_x, &m_y);
	return glm::vec2(m_x, m_y);
}

double InternalMouseHandler::getOffset()
{
	return offset;
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
	glfwSetScrollCallback(this->window, callback);
}