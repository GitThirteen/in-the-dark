#include "Camera.h"

Camera::Camera() : Camera(1.0) { }

Camera::Camera(double radius) 
{
	this->camera = CameraCoords(
		glm::vec3(0.0f, 0.0f, radius),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	this->radius = radius;
	this->mouse_old = { NAN, NAN };

	this->view_mat = calcViewMatrix();
	this->proj_mat = calcProjMatrix();
}

Camera::Camera(glm::vec3& origin, glm::vec3& target, glm::vec3& up, double radius)
{
	this->camera = CameraCoords(origin, target, up);
	this->radius = radius;
	this->mouse_old = { NAN, NAN };

	this->view_mat = calcViewMatrix();
	this->proj_mat = calcProjMatrix();
}

void Camera::update(bool rightMouseDown, glm::vec2 mouse, double r)
{
	if (this->radius != r)
	{
		this->camera.spherical.x = r;
		this->radius = r;

		this->camera.origin = this->camera.calcCartesian() + this->camera.target;
		this->view_mat = calcViewMatrix();
	}

	if (!this->locked && rightMouseDown)
	{
		if (std::isnan(this->mouse_old.x)) this->mouse_old.x = mouse.x;
		if (std::isnan(this->mouse_old.y)) this->mouse_old.y = mouse.y;

		double dx = mouse.x - this->mouse_old.x;
		double dy = mouse.y - this->mouse_old.y;

		double width = SettingsManager::getInstance().get<int>("width");
		double height = SettingsManager::getInstance().get<int>("height");
		double move_x = dx / (width * 0.5);
		double move_y = dy / (height * 0.5);

		if (dx != 0 || dy != 0)
		{
			// adjusting pitch if mouse moves up or down
			this->camera.spherical.y = util::clamp(this->camera.spherical.y - move_y, 0.01, M_PI - 0.01);
			// adjusting yaw if mouse moves left or right
			this->camera.spherical.z += move_x;

			this->camera.origin = this->camera.calcCartesian() + this->camera.target;
			this->view_mat = calcViewMatrix();
		}
	}

	this->mouse_old.x = mouse.x;
	this->mouse_old.y = mouse.y;
}

void Camera::lock()
{
	if (this->locked) return;
	this->locked = !this->locked;
}

void Camera::unlock()
{
	if (!this->locked) return;
	this->locked = !this->locked;
}

glm::mat4 Camera::calcViewMatrix()
{
	return glm::lookAt(this->camera.origin, this->camera.target, this->camera.up);
}

glm::mat4 Camera::calcProjMatrix()
{
	SettingsManager& settings = SettingsManager::getInstance();

	int height = settings.get<int>("height");
	int width = settings.get<int>("width");
	float fov = settings.get<float>("fov");
	float near = settings.get<float>("near");
	float far = settings.get<float>("far");

	return glm::perspective(glm::radians(fov), width / (float) height, near, far);
}

glm::mat4 Camera::getViewProjMatrix()
{
	return this->proj_mat * this->view_mat;
}

// CameraCoords

CameraCoords::CameraCoords() { }

CameraCoords::CameraCoords(glm::vec3& origin, glm::vec3& target, glm::vec3& up)
{
	this->origin = origin;
	this->target = target;
	this->up = up;
	this->spherical = calcSpherical();
}

glm::vec3 CameraCoords::calcSpherical()
{
	float x = this->origin.x;
	float y = this->origin.y;
	float z = this->origin.z;

	auto r = sqrt(x * x + y * y + z * z);
	return glm::vec3(
		r,				// radius (r)
		acos(y / r),	// inclination (theta)
		atan2(z, x)		// azimuth (phi)
	);
}

glm::vec3 CameraCoords::calcCartesian()
{
	float r = this->spherical.x;
	float incl = this->spherical.y;
	float azi = this->spherical.z;

	return glm::vec3(
		r * cos(azi) * sin(incl),
		r * cos(incl),
		r * sin(azi) * sin(incl)
	);
}