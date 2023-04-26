#include "GameState.h"
#include <iostream>
#include <string>

class TestState : public GameState
{
	ObjData stone;

	void init() override
	{
		this->stone = assets.getObj(Object::Stone);
	}

	void update() override
	{

	}

	void draw() override
	{
		// TODO: Consider abstracting methods away in separate class handling drawing operations so glew and glfw aren't "global"
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// TODO: @Future Me -> Definitely abstract this away
		glBufferData(GL_ARRAY_BUFFER, this->stone.v.size() * sizeof(glm::vec3), &this->stone.v[0], GL_STATIC_DRAW);
	}

	void discard() override
	{

	}
};
