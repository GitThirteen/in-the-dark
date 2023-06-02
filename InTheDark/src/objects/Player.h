#pragma once

// WARN: Do NOT switch these includes around!
#include "GameObject.h"
#include "../managers/EventHandler.h"
#include "../managers/Clock.h"

class Player : public GameObject
{
public:
	void update();
	void move();
private:
	//float velocity = 0.0f;
	float speed = 2.0f;
	glm::vec3 movementVector = glm::vec3(0);

	EventHandler& events = EventHandler::getInstance();
	Clock& clock = Clock::getInstance();
};