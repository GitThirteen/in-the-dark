#pragma once

// WARN: Do NOT switch these #include's around!
#include "GameObject.h"
#include "../managers/EventHandler.h"
#include "../managers/Clock.h"
#include "../Camera.h"

class Player : public GameObject
{
public:
	void update(const glm::vec3&);
	bool isCollidingWith(std::shared_ptr<GameObject>);
	bool isColliding();
	std::vector<std::shared_ptr<GameObject>> getCollisions();
	void setLevelObjs(std::vector<std::shared_ptr<GameObject>>&);
	void kill();
	bool hasLost();

private:
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 input_direction = glm::vec3(0.0f);
	std::vector<std::shared_ptr<GameObject>>* level_objs;

	bool is_grounded = true;
	int lives = 3;
	
	const float GRAVITY = -9.81f;
	const float JUMP_MULTIPLIER = 5.0f;
	const float FALL_MULTIPLIER = 1.5f;
	const float MAX_SPEED = 4.0f; // TODO: SET THIS PER ASSET!!!
	float TRANSITION_SPEED = 4.0f; // TODO: SET THIS PER ASSET!!!
 
	EventHandler& events = EventHandler::getInstance();
	Clock& clock = Clock::getInstance();

	glm::vec3 calcForce();
	glm::vec3 calcJump();
};