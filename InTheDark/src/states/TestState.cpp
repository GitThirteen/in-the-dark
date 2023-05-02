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
		canvas.clear();

		
		// TODO: @Future Me -> Definitely abstract this away
		glBufferData(GL_ARRAY_BUFFER, this->stone.v.size() * sizeof(glm::vec3), &this->stone.v[0], GL_STATIC_DRAW);
	}

	void discard() override
	{

	}
};
