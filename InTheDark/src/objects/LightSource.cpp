 #include "LightSource.h"

void lightSource::Directional::addToScene()
{
	if (this->placed) return;

	ShaderManager& shader = ShaderManager::getInstance();
	shader.set(ShaderLocation::Default::LIGHT_D_COL, this->color);
	shader.set(ShaderLocation::Default::LIGHT_D_DIRECTION, this->direction);

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