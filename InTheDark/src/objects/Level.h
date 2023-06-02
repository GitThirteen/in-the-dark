#pragma once

#include "../states/GameState.h"

class Level : public GameState
{
public:
	Level(uint16_t level_number)
	{
		this->level = assets.getLevel(level_number);
	};
protected:
	lightSource::Directional directionalLight;
	PointLightDataHandler pointLights;
	
};