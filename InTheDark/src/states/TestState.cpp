#include "GameState.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include "../objects/ParticleSystem.h"
#include "../objects/Torch.h"

class TestState : public GameState
{
	LevelWrapper level;
	std::vector<Torch> torches;

	void init() override
	{
		this->level = assets.getLevel(0);

		initCamera();

		auto cam = std::make_shared<Camera>(this->camera);
		for (auto& obj : this->level.data)
		{
			if (obj->asset.type == AssetType::TORCH)
			{
				Torch t = Torch(obj);
				t.setCamera(cam);
				torches.push_back(t);
			}
		}

		this->level.lights.directional_light.addToScene();
		this->level.lights.point_lights.addToScene();
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

		this->level.player->update(camera.coords.target - camera.coords.origin);

		camera.updatePosition(this->level.player->position);
		
		auto collisions = std::vector<std::shared_ptr<GameObject>>();
		for (auto& obj : this->level.data)
		{
			if (
				obj->asset.type == AssetType::PLAYER ||
				obj->asset.type == AssetType::TORCH
			) continue;
			//if (obj->isGround) continue;
			
			if (this->level.player->isCollidingWith(obj))
			{
				collisions.push_back(obj);
			}
		}

		for (auto& c : collisions)
		{
			glm::vec3 p_ctr_pos = this->level.player->position;
			p_ctr_pos.y += this->level.player->bbox.upper.y * 0.5f;

			glm::vec3 o_ctr_pos = c->position;
			o_ctr_pos.y += c->bbox.upper.y * 0.5f;

			auto collision_vector = o_ctr_pos - p_ctr_pos;
			auto cv_abs = glm::abs(collision_vector);

			//glm::vec3 p_half_width = (this->level.player->bbox.upper - this->level.player->bbox.lower) * 0.5f;
			//glm::vec3 o_half_width = (c->bbox.upper - c->bbox.lower) * 0.5f;

			//auto overlap = p_half_width + o_half_width - glm::abs(collision_vector);
			auto smallest = std::min({ cv_abs.x, cv_abs.y, cv_abs.z });

			//auto collision_normal = glm::normalize(collision_vector);
			auto move_vector = glm::vec3(0.0f);
			if (smallest == cv_abs.x)
				move_vector.x = collision_vector.x;
			else if (smallest == cv_abs.y)
				move_vector.y = collision_vector.y;
			else if (smallest == cv_abs.z)
				move_vector.z = collision_vector.z;

			//std::cout << std::to_string(collision_vector.x) + " " + std::to_string(collision_vector.y) + " " + std::to_string(collision_vector.z) << std::endl;

			this->level.player->position -= move_vector;
			this->level.player->asset.translate(-move_vector);
		}
	}

	void draw() override
	{
		if (!canvas.post_processor.isCreated())
		{
			canvas.post_processor.create();
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

	void resolveCollision()
	{

	}
};
