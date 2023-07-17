#include "PointLightData.h"

bool PointLightData::incomplete()
{
	return this->entities.empty();
}

void PointLightData::create()
{
	if (incomplete())
	{
		LOG_F(ERROR, "PointLight data incomplete, PointLights have not been created.");
		return;
	}

	std::vector<lightSource::Point> point_lights;
	for (auto& torch : this->entities)
	{
		point_lights.push_back(torch.light);
	}

	GLuint ssbo;
	glGenBuffers(1, &ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, point_lights.size() * sizeof(lightSource::Point), point_lights.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ShaderLocation::Default::Bindings::LIGHT_P_BUFFER, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	this->ubo = ssbo;
	this->created = true;
}

void PointLightData::addToScene()
{
	if (!this->created)
	{
		LOG_F(ERROR, "Attempted to add point lights to shader before creation. Make sure to call 'create()' before adding them.");
		return;
	}

	std::vector<lightSource::Point> point_lights;
	for (auto& torch : this->entities)
	{
		torch.light.isActive = glm::vec3(torch.isActive);
		point_lights.push_back(torch.light);
	}

	// Uncomment if dynamic updating required
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, point_lights.size() * sizeof(lightSource::Point), point_lights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}