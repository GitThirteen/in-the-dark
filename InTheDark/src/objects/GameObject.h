# pragma once

#include "GameAsset.h"
#include "BBox.h"

class GameObject
{
public:
	GameAsset asset;
	glm::vec3 position;
	BBox bbox;

	virtual void update() { };

	BBox getTrueBBox()
	{
		BBox b{ };
		b.lower = this->bbox.lower + this->position;
		b.upper = this->bbox.upper + this->position;
		return b;
	}

private:

};