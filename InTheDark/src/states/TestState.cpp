#include "GameState.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include "../objects/ParticleSystem.h"

class TestState : public GameState
{
	LevelWrapper level;
	ParticleSystem particles = ParticleSystem(glm::vec3(1.0f));

	void init() override
	{
		this->level = assets.getLevel(0);

		initCamera();

		this->level.lights.directionalLight.addToScene();
		this->level.lights.pointLights.addToScene();
	}

	void update() override
	{
		//std::cout << "dt: " + std::to_string(clock.getDeltaTime()) + " sec." << std::endl;
		//std::cout << "fps: " + std::to_string(clock.getFPS()) << std::endl;

		events.poll();

		bool mouse_pressed = events.mouse.pressed(GLFW_MOUSE_BUTTON_RIGHT);
		glm::vec2 mouse_pos = events.mouse.getPosition();
		double cam_radius = events.mouse.getOffset();
		camera.update(mouse_pressed, mouse_pos, cam_radius);

		this->level.player->update();

		camera.updatePosition(this->level.player->position);
		
		auto collisions = std::vector<std::shared_ptr<GameObject>>();
		for (auto& obj : this->level.data)
		{
			if (obj->asset.type == AssetType::PLAYER) continue;
			
			if (this->level.player->isCollidingWith(obj))
			{
				collisions.push_back(obj);
			}
		}

		//std::cout << collisions.size() << std::endl;

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

		this->particles.render();
	}

	void discard() override
	{

	}

private:
	void initCamera()
	{
		glm::vec3 cam_start_pos = glm::vec3(0.0, 20.0, 20.0);
		glm::vec3 player_pos = this->level.player->position;
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		double radius = sqrt(
			(cam_start_pos.x - player_pos.x) * (cam_start_pos.x - player_pos.x) +
			(cam_start_pos.y - player_pos.y) * (cam_start_pos.y - player_pos.y) +
			(cam_start_pos.z - player_pos.z) * (cam_start_pos.z - player_pos.z)
		);
		camera = Camera(cam_start_pos, player_pos, up, radius);
		events.mouse.setOffset(radius);

		camera.unlock(); // TODO remove and properly use lock & unlock once main menu and co are defined
	}
};
