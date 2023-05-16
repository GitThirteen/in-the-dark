#pragma once

#include <glm/glm.hpp>

#include "../managers/ShaderManager.h"

class LightSource
{
protected:
	glm::vec3 color = glm::vec3(0);

	LightSource() { };

	virtual void place() = 0;

	void setColor(glm::vec3);
};

class PointLight : public LightSource
{
public:
	PointLight();
	PointLight(const glm::vec3 position, const glm::vec3 attenuation);

	void place() override;

	void setPosition(const glm::vec3);
	void setAttenuation(const glm::vec3);
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