#include "LightSource.h"

void LightSource::setColor(glm::vec3 color)
{
	this->color = color;
}

// Point Light

PointLight::PointLight() { };
PointLight::PointLight(glm::vec3 position, glm::vec3 attenuation)
{
	this->position = position;
	this->attenuation = attenuation;
}

void PointLight::setPosition(const glm::vec3 position)
{
	this->position = position;
}

void PointLight::setAttenuation(glm::vec3 attenuation)
{
	this->attenuation = attenuation;
}

void PointLight::place()
{
	ShaderManager& shader = ShaderManager::getInstance();

	// TODO: Set Color, Position and Attenuation
}

// Directional Light

DirectionalLight::DirectionalLight() { };
DirectionalLight::DirectionalLight(glm::vec3 direction)
{
	this->direction = direction;
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

void DirectionalLight::place()
{
	ShaderManager& shader = ShaderManager::getInstance();

	// TODO: Set Color and Direction
}