#include "GameState.h"
#include <iostream>
#include <string>

class TestState : public GameState
{
	LevelWrapper level;
	float rot;
	float cur_rot = 0;

	void init() override
	{
		this->level = assets.getLevel(0);

		this->level.lights.directionalLight.addToScene();
		this->level.lights.pointLights.addToScene();
		
		/*this->rot = 0.005f;
		this->stone = assets.getObj(Object::TREASURE);
		//this->stone.rotate(glm::vec3(0.0f, 1.0f, 0.0f), 270);
		this->stone.illuminate(glm::vec3(0.1, 0.4, 0.1), 32);
		this->stone.translate(glm::vec3(0, 1, 0));

		this->stone2 = assets.getObj(Object::STONE);
		this->stone2.illuminate(glm::vec3(0.1, 0.4, 0.0));*/
	}

	void update() override
	{
		//this->cur_rot += rot;

		//std::cout << "Current rotation: " + std::to_string(cur_rot) + " deg." << std::endl;
		//std::cout << "dt: " + std::to_string(clock.getDeltaTime()) + " sec." << std::endl;
		std::cout << "fps: " + std::to_string(clock.getFPS()) << std::endl;

		if (this->cur_rot >= 360.0f)
		{
			this->cur_rot = 0.0f;
		}
	}

	void draw() override
	{
		canvas.clear(24, 34, 33);

		for (auto& obj : this->level.data)
		{
			obj->asset.draw();
		}
	}

	void discard() override
	{

	}
};
