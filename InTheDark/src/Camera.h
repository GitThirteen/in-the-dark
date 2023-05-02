#pragma once

#define _USE_MATH_DEFINES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <cmath>

#include "managers/SettingsManager.h"

struct CameraCoords
{
	glm::vec3 origin = glm::vec3();
	glm::vec3 target = glm::vec3();
	glm::vec3 up = glm::vec3();

	glm::vec3 spherical = glm::vec3();

	CameraCoords();
	CameraCoords(glm::vec3&, glm::vec3&, glm::vec3&);

	glm::vec3 calcSpherical();
	glm::vec3 calcCartesian();
};

class Camera
{
public:
	Camera();
	Camera(double);
	Camera(glm::vec3&, glm::vec3&, glm::vec3&, double);

	void update(bool, glm::vec2, double);
	void lock();
	void unlock();

	glm::mat4& getViewProjMatrix();

private:
	double radius = 1.0;
	bool locked = true;
	CameraCoords camera;
	glm::mat4 view_mat;
	glm::mat4 proj_mat;

	glm::vec2 mouse_old;

	glm::mat4 calcViewMatrix();
	glm::mat4 calcProjMatrix();
};