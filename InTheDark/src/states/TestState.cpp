#include "GameState.h"
#include <iostream>
#include <string>

class TestState : public GameState
{
	GameObject stone;

	void init() override
	{
		// Maybe let GameState (or level in the future, which should be a GameState child) hold all GameObjects by default?
		// Compare to already existing manager instances
		this->stone = assets.getObj(Object::STONE);
	}

	void update() override
	{

	}

	void draw() override
	{
		canvas.clear();
		this->stone.draw();
	}

	void discard() override
	{

	}
};
