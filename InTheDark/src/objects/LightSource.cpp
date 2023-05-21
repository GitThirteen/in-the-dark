 #include "LightSource.h"

void LightSource::setColor(uint8_t r, uint8_t g, uint8_t b)
{
	float rgb_r = r / 255.0f;
	float rgb_g = g / 255.0f;
	float rgb_b = b / 255.0f;

	this->color = glm::vec3(rgb_r, rgb_g, rgb_b);
}

// Point Light

PointLight::PointLight() { };
PointLight::PointLight(glm::vec3 position, glm::vec3 attenuation)
{
	this->position = position;
	this->attenuation = attenuation;
}

void PointLight::setPosition(double x, double y, double z)
{
	this->position = glm::vec3(x, y, z);
}

void PointLight::setAttenuation(glm::vec3 attenuation)
{
	this->attenuation = attenuation;
}

void PointLight::place()
{
	ShaderManager& shader = ShaderManager::getInstance();

	shader.set(ShaderLocation::LIGHT_P_COL, this->color);
	shader.set(ShaderLocation::LIGHT_P_POSITION, this->position);
	shader.set(ShaderLocation::LIGHT_P_ATTENUATION, this->attenuation);
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

	shader.set(ShaderLocation::LIGHT_D_COL, this->color);
	shader.set(ShaderLocation::LIGHT_D_DIRECTION, this->direction);
}