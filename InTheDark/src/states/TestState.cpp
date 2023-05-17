#include "GameState.h"
#include <iostream>
#include <string>

class TestState : public GameState
{
	GameObject stone;
	Lights lights;
	float rot;
	float cur_rot = 0;

	void init() override
	{
		// Maybe let GameState (or level in the future, which should be a GameState child) hold all GameObjects by default?
		// Compare to already existing manager instances
		this->rot = 0.005f;
		this->stone = assets.getObj(Object::STONE);
		this->stone.illuminate(glm::vec3(0.1, 0.7, 0.3), 8);

		this->lights = assets.getLights();
		for (auto& light : this->lights) light->place();
	}

	void update() override
	{
		this->cur_rot += rot;

		std::cout << "Current rotation: " + std::to_string(cur_rot) + " deg." << std::endl;
		std::cout << "dt: " + std::to_string(clock.getDeltaTime()) + " sec." << std::endl;
		std::cout << "fps: " + std::to_string(clock.getFPS()) << std::endl;

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
