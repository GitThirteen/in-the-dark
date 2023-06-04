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
	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ShaderManager::getInstance().use("shader");

	if (this->active_state)
	{
		this->active_state->draw();
	}

	glDisable(GL_DEPTH_TEST);

	canvas.post_processor.unbind();
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