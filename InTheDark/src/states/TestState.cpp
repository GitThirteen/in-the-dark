#include "GameState.h"
#include <iostream>
#include <string>

class TestState : public GameState
{
	GameObject stone;
	float rot;
	float cur_rot = 0;

	void init() override
	{
		// Maybe let GameState (or level in the future, which should be a GameState child) hold all GameObjects by default?
		// Compare to already existing manager instances
		this->rot = 0.005f;
		this->stone = assets.getObj(Object::STONE);
	}

	void update() override
	{
		this->cur_rot += rot;

		std::cout << "Current rotation: " + std::to_string(cur_rot) + " deg." << std::endl;

		if (this->cur_rot >= 360.0f)
		{
			this->cur_rot = 0.0f;
		}
	}

	void draw() override
	{
		canvas.clear();
		this->stone.rotate(glm::vec3(0, 1, 0), this->rot);
		this->stone.draw();
	}

	void discard() override
	{

	}
};
