#include "GameStateManager.h"

void GameStateManager::changeState(std::unique_ptr<GameState> state)
{
	this->discard();

	state->init();
	this->active_state.swap(state);
}

void GameStateManager::update()
{
	if (this->active_state)
	{
		this->active_state->update();
	}
}

void GameStateManager::draw() // TODO don't pass radius in as param but rather have some sort of global entity tracking that, maybe same for mouse position
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwPollEvents();

	this->camera.unlock(); // TODO remove and properly use lock & unlock once main menu and co are defined
	
	double m_x = 0.0, m_y = 0.0;
	glfwGetCursorPos(this->window, &m_x, &m_y);
	bool rotateFlag = (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	this->camera.update(rotateFlag, glm::vec2(m_x, m_y), 6.0f); // <-- replace 6.0f with radius

	ShaderManager::getInstance().set(ShaderLocation::VIEWPROJECTION_MAT, this->camera.getViewProjMatrix());

	if (this->active_state)
	{
		this->active_state->draw();
	}

	glfwSwapBuffers(this->window);
}

void GameStateManager::discard()
{
	if (this->active_state)
	{
		this->active_state->discard();
	}
}