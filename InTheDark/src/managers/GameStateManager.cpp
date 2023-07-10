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
	if (this->active_state)
	{
		this->active_state->draw();
	}
}

void GameStateManager::discard()
{
	if (this->active_state)
	{
		this->active_state->discard();
	}
}