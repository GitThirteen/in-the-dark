#pragma once

#include <glm/glm.hpp>

#include "../managers/ShaderManager.h"

class LightSource
{
public:
	void setColor(uint8_t r, uint8_t g, uint8_t b);
	virtual void place() = 0;
protected:
	glm::vec3 color = glm::vec3(0);

	LightSource() { };	
};

class PointLight : public LightSource
{
public:
	PointLight();
	PointLight(glm::vec3 position, glm::vec3 attenuation);

	void place() override;

	void setPosition(double x, double y, double z);
	void setAttenuation(glm::vec3);
private:
	glm::vec3 position = glm::vec3(0);
	glm::vec3 attenuation = glm::vec3(0);
};

class DirectionalLight : public LightSource
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 direction);

	void place() override;

	void setDirection(glm::vec3);
private:
	glm::vec3 direction = glm::vec3(0);
};