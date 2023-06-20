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
	if (!canvas.post_processor.isCreated())
	{
		canvas.post_processor.create();
	}

	canvas.post_processor.bind();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (this->active_state)
	{
		this->active_state->draw();
	}

	canvas.post_processor.unbind();
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	canvas.post_processor.draw();
	
	glfwSwapBuffers(this->window);
}

void GameStateManager::discard()
{
	if (this->active_state)
	{
		this->active_state->discard();
	}
}