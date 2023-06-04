#include "GameState.h"
#include <typeinfo>
#include <iostream>
#include <string>

class TestState : public GameState
{
	LevelWrapper level;
	float rot;

	void init() override
	{
		this->level = assets.getLevel(0);

		this->level.lights.directionalLight.addToScene();
		this->level.lights.pointLights.addToScene();
	}

	void update() override
	{
		//this->cur_rot += rot;

		//std::cout << "Current rotation: " + std::to_string(cur_rot) + " deg." << std::endl;
		//std::cout << "dt: " + std::to_string(clock.getDeltaTime()) + " sec." << std::endl;
		//std::cout << "fps: " + std::to_string(clock.getFPS()) << std::endl;

		this->level.player->update();
		
		auto collisions = std::vector<std::shared_ptr<GameObject>>();
		for (auto& obj : this->level.data)
		{
			if (obj->asset.type == AssetType::PLAYER) continue;
			
			if (this->level.player->isCollidingWith(obj))
			{
				collisions.push_back(obj);
			}
		}

		std::cout << collisions.size() << std::endl;

		if (collisions.size() == 1)
		{

		}
		else if (collisions.size() > 1)
		{

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
