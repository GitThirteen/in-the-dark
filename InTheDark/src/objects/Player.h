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

private:
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 input_direction = glm::vec3(0.0f);
	std::vector<std::shared_ptr<GameObject>>* level_objs;

	const float GRAVITY = -9.81;
	const float MAX_SPEED = 4.0f; // TODO: SET THIS PER ASSET!!!
	float TRANSITION_SPEED = 4.0f; // TODO: SET THIS PER ASSET!!!
 
	EventHandler& events = EventHandler::getInstance();
	Clock& clock = Clock::getInstance();

	glm::vec3 calcForce();
	void resetPosition();
};