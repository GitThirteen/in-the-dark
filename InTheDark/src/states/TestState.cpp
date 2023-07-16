#include "GameState.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include "../objects/ParticleSystem.h"
#include "../objects/Torch.h"

struct Spikes {
	GameObjects objs;
	float default_y = FLT_MIN;
	float clock_default = 0.0f;
	float spike_clock = 0.0f;

	glm::vec3 get_target_offset()
	{
		auto time = fmod(this->spike_clock, 5.0f);
		if (time < 1.0f)
			return glm::vec3(0.0f, time / 1.0f, 0.0f);
		if (time < 2.5f)
			return glm::vec3(0.0f, 1.0f, 0.0f);
		if (time < 3.5f)
			return glm::vec3(0.0f, (3.5f - time), 0.0f);
		else
			return glm::vec3(0.0f);
	}
};

class TestState : public GameState
{
	LevelWrapper level;
	std::vector<Torch> torches;
	std::array<Spikes, 4> spikes;

	void init() override
	{
		this->level = assets.getLevel(0);
		this->level.player->setLevelObjs(this->level.data);

		initCamera();

		for (auto& obj : this->level.data)
		{
			if (obj->asset.type == AssetType::TORCH)
			{
				Torch t = Torch(obj);
				t.setCamera(this->camera);
				torches.push_back(t);
			}

			if (obj->asset.type == AssetType::SPIKES)
			{
				int group_index = 3;
				if (obj->position.x == 0)		group_index = 0;
				else if (obj->position.x == -1) group_index = 1;
				else if (obj->position.x == -2) group_index = 2;

				this->spikes[group_index].objs.push_back(obj);

				auto y_pos = this->spikes[group_index].default_y;
				if (y_pos == FLT_MIN)
				{
					this->spikes[group_index].default_y = obj->position.y;
				}
			}
		}

		auto spike_default = std::array<float, 4> { 0.0f, -0.5f, -1.0f, 0.25f };
		for (int i = 0; i < spike_default.size(); i++)
		{
			this->spikes[i].spike_clock = 
				this->spikes[i].clock_default = spike_default[i];
		}

		this->level.lights.directional_light.addToScene();
		this->level.lights.point_lights.addToScene();
	}

	void update() override
	{
		//std::cout << "dt: " + std::to_string(clock.getDeltaTime()) + " sec." << std::endl;
		//std::cout << "fps: " + std::to_string(clock.getFPS()) << std::endl;

		events.poll();

		if (this->level.player->hasLost()) return;

		bool mouse_pressed = events.mouse.pressed(GLFW_MOUSE_BUTTON_RIGHT);
		glm::vec2 mouse_pos = events.mouse.getPosition();
		double cam_radius = events.mouse.getOffset();
		camera->update(mouse_pressed, mouse_pos, cam_radius);

		// Update environment

		auto dt = clock.getDeltaTime();

		for (auto& spike_group : this->spikes)
		{
			spike_group.spike_clock += dt;
			auto offset = spike_group.get_target_offset();

			for (auto& spike : spike_group.objs)
			{
				auto target_pos = glm::vec3(spike->position.x, spike_group.default_y, spike->position.z) + offset;
				auto delta = target_pos - spike->position;
				spike->position = target_pos;
				spike->asset.translate(delta);
			}
		}

		// Update player

		this->level.player->update(camera->coords.target - camera->coords.origin);

		// Check for dmg collisions

		for (auto& group : this->spikes)
		{
			for (auto& spike : group.objs)
			{
				if (this->level.player->isCollidingWith(spike))
				{
					this->level.player->kill();
				}
			}
		}

		camera->updatePosition(this->level.player->position);
	}

	void draw() override
	{
		if (!canvas.post_processor.isCreated())
		{
			canvas.post_processor.create();
		}

		if (this->level.player->hasLost())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			canvas.clear(255, 0, 0);
			canvas.window.swapBuffers();
			return;
		}
		
		canvas.clear(24, 34, 33);

		// First pass: Render stage

		canvas.post_processor.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		
		for (auto& obj : this->level.data)
		{
			if (
				obj->asset.type == AssetType::PLAYER || 
				obj->asset.type == AssetType::TORCH
			) continue;

			obj->asset.draw();
		}

		for (auto& torch : this->torches)
		{
			torch.draw();
		}

		// Second pass: Render character (TODO: With post-processing)

		this->level.player->asset.draw();

		canvas.post_processor.unbind();
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		canvas.post_processor.draw();

		// Third pass: Render particles

		for (auto& torch : this->torches)
		{
			torch.emitSmoke();
		}

		canvas.window.swapBuffers();
	}

	void discard() override
	{

	}

private:
	void initCamera()
	{
		glm::vec3 cam_start_pos = glm::vec3(0.0, 15.0, 15.0);
		glm::vec3 player_pos = this->level.player->position;
		glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
		double radius = sqrt(
			(cam_start_pos.x - player_pos.x) * (cam_start_pos.x - player_pos.x) +
			(cam_start_pos.y - player_pos.y) * (cam_start_pos.y - player_pos.y) +
			(cam_start_pos.z - player_pos.z) * (cam_start_pos.z - player_pos.z)
		);
		camera = std::make_shared<Camera>(Camera(cam_start_pos, player_pos, up, radius));
		events.mouse.setOffset(radius);

		camera->unlock(); // TODO remove and properly use lock & unlock once main menu and co are defined
	}
};
