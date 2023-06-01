#pragma once

#include <glm/glm.hpp>

#include "../util/json.hpp"
#include "../managers/ShaderManager.h"

#define FLOAT_PADDING(x) float x = 0.0f;

namespace lightSource
{
	struct Directional
	{
		glm::vec3 color = glm::vec3(0);
		glm::vec3 direction = glm::vec3(0);
		bool placed = false;

		void addToScene();
	};

	struct Point
	{
		glm::vec3 color = glm::vec3(0);
		FLOAT_PADDING(c)
		glm::vec3 position = glm::vec3(0);
		FLOAT_PADDING(p)
		glm::vec3 attenuation = glm::vec3(0);
		FLOAT_PADDING(a)

		// TODO might need own addToScene() since player light is dynamic
	};

	void to_json(nlohmann::json&, const lightSource::Directional&) = delete;
	void from_json(const nlohmann::json&, lightSource::Directional&);

	void to_json(nlohmann::json& j, const lightSource::Point&) = delete;
	void from_json(const nlohmann::json& j, lightSource::Point&);
}

struct PointLightDataHandler
{
	GLuint ubo;
	std::vector<lightSource::Point> entities;
	bool created = false;
	bool placed = false;

	bool incomplete();
	void create();
	void addToScene();
};