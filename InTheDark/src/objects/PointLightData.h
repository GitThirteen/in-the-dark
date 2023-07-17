#pragma once
#include <vector>
#include "Torch.h"

class PointLightData
{
public:
	GLuint ubo;
	std::vector<Torch> entities;
	//std::vector<lightSource::Point> entities;
	bool created = false;

	bool incomplete();
	void create();
	void addToScene();
};