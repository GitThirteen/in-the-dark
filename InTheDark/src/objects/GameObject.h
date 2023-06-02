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

private:

};