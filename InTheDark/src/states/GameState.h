#pragma once

#include "../managers/AssetManager.h"
#include "../managers/CanvasManager.h"
#include "../managers/Clock.h"
#include "../Camera.h"

class GameState
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void discard() = 0;

	virtual ~GameState() { };

protected:
	AssetManager& assets = AssetManager::getInstance();
	CanvasManager& canvas = CanvasManager::getInstance();
	Clock& clock = Clock::getInstance();
	EventHandler& events = EventHandler::getInstance();
	std::shared_ptr<Camera> camera = nullptr;
};
