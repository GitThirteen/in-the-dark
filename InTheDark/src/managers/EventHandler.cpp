#include "EventHandler.h"

void EventHandler::poll()
{
	glfwPollEvents();
}

// Mouse Handler

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