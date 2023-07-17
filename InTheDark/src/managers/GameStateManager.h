#pragma once

#include <memory>

#include "../states/GameState.h"
#include "../Camera.h"

class GameStateManager
{
public:
	void changeState(std::unique_ptr<GameState> state);
	void update();
	void draw();
	void discard();

	static GameStateManager& getInstance()
	{
		static GameStateManager instance;
		return instance;
	};

	GameStateManager(GameStateManager const&)	= delete;
	void operator=(GameStateManager const&)		= delete;

private:
	std::unique_ptr<GameState> active_state = nullptr;

	Clock& clock = Clock::getInstance();
	CanvasManager& canvas = CanvasManager::getInstance();
	EventHandler& events = EventHandler::getInstance();

	GameStateManager() { }
};