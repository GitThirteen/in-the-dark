 #include "LightSource.h"

void lightSource::Directional::addToScene()
{
	if (this->placed) return;

	ShaderManager& shader = ShaderManager::getInstance();
	shader.set(ShaderLocation::LIGHT_D_COL, this->color);
	shader.set(ShaderLocation::LIGHT_D_DIRECTION, this->direction);

	this->placed = true;
}

void lightSource::from_json(const nlohmann::json& j, lightSource::Directional& d)
{
	auto& dir = j.at("direction");
	d.direction = glm::vec3(dir[0], dir[1], dir[2]);
	auto& col = j.at("color");
	d.color = glm::vec3(col[0] / 255.0f, col[1] / 255.0f, col[2] / 255.0f);
}

void lightSource::from_json(const nlohmann::json& j, lightSource::Point& p)
{
	auto& pos = j.at("position");
	p.position = glm::vec3(pos[0], pos[1], pos[2]);
	auto& col = j.at("color");
	p.color = glm::vec3(col[0] / 255.0f, col[1] / 255.0f, col[2] / 255.0f);
	auto& att = j.at("attenuation");
	p.attenuation = glm::vec3(att[0], att[1], att[2]);
}

// POINT LIGHT DATA HANDLER

bool PointLightDataHandler::incomplete()
{
	return this->entities.empty();
}

void PointLightDataHandler::create()
{
	if (incomplete())
	{
		LOG_F(ERROR, "PointLight data incomplete, PointLights have not been created.");
		return;
	}

	GLuint ssbo;
	glGenBuffers(1, &ssbo);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->entities.size() * sizeof(lightSource::Point), this->entities.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ShaderBinding::LIGHT_P_BUFFER, ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	this->ubo = ssbo;
	this->created = true;
}

void PointLightDataHandler::addToScene()
{
	if (this->placed) return;

	if (!this->created)
	{
		LOG_F(ERROR, "Attempted to add point lights to shader before creation. Make sure to call 'create()' before adding them.");
		return;
	}

	// Uncomment if dynamic updating required
	//glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, this->entities.size() * sizeof(lightSource::Point), this->entities.data());
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	this->placed = true;
}