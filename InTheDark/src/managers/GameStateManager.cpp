#include "GameStateManager.h"

void GameStateManager::changeState(std::unique_ptr<GameState> state)
{
	this->discard();

	state->init();
	this->active_state.swap(state);
}

void GameStateManager::update()
{
	clock.tick();

	if (this->active_state)
	{
		this->active_state->update();
	}
}

void GameStateManager::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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