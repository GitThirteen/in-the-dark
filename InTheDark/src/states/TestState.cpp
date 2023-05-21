#include "GameState.h"
#include <iostream>
#include <string>

class TestState : public GameState
{
	GameObject stone;
	GameObject stone2;
	Lights lights;
	float rot;
	float cur_rot = 0;

	void init() override
	{
		this->rot = 0.005f;
		this->stone = assets.getObj(Object::TREASURE);
		//this->stone.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 270);
		this->stone.illuminate(glm::vec3(0.1, 0.4, 0.1), 32);
		this->stone.translate(glm::vec3(0, 1, 0));

		this->stone2 = assets.getObj(Object::STONE);
		this->stone2.illuminate(glm::vec3(0.1, 0.4, 0.0));

		this->lights = assets.getLights();
		for (auto& light : this->lights) light->place();
	}

	void update() override
	{
		//this->cur_rot += rot;

		//std::cout << "Current rotation: " + std::to_string(cur_rot) + " deg." << std::endl;
		//std::cout << "dt: " + std::to_string(clock.getDeltaTime()) + " sec." << std::endl;
		//std::cout << "fps: " + std::to_string(clock.getFPS()) << std::endl;

		if (this->cur_rot >= 360.0f)
		{
			this->cur_rot = 0.0f;
		}
	}

	void draw() override
	{
		canvas.clear(29, 24, 21);
		//this->stone.rotate(glm::vec3(0, 1, 0), this->rot);
		this->stone.draw();
		this->stone2.draw();
	}

	void discard() override
	{

	}
};
