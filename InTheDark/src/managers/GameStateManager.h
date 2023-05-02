#pragma once

#include <memory>
#include "../states/GameState.h"
#include "../Camera.h"
#include "ShaderManager.h"

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

	void setWindow(GLFWwindow* window)
	{
		this->window = window;
	};

	GameStateManager(GameStateManager const&)	= delete;
	void operator=(GameStateManager const&)		= delete;

private:
	std::unique_ptr<GameState> active_state = nullptr;
	GLFWwindow* window = nullptr;
	Camera camera = Camera(1.0);

	GameStateManager() { }
};