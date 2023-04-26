#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	Camera(double);
	Camera(glm::vec3, glm::vec3, glm::vec3, double);

	void update();

private:

};