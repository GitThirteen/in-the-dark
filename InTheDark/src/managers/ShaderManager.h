#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "../util/util.h"

enum Shader
{
	Vertex,
	Fragment,
	Geometry
};

enum ShaderLocation {
	POSITION			= 0,
	NORMAL				= 1,
	UV					= 2,
	TRANSFORM_MAT		= 3,
	VIEWPROJECTION_MAT	= 4,
	LIGHT_D_DIRECTION	= 5,
	LIGHT_D_COL			= 6,
	PPS_COLOR_TEXTURE	= 7,
	PPS_DEPTH_TEXTURE	= 8,
	CAMERA_POSITION		= 10,
	REFLECTION			= 11,
	GLOSSINESS			= 12,
	TEXTURE				= 13,
	SCREEN_WIDTH		= 14,
	SCREEN_HEIGHT		= 15,
	DELTA_TIME			= 16,
	PARTICLE_POSITION	= 17,
	PARTICLE_VELOCITY	= 18,
	PARTICLE_AGE		= 19,
	PARTICLE_COLOR		= 20,
	PARTICLE_LIFETIME	= 21,
	PARTICLE_SYS_POS	= 22,
};

enum ShaderBinding {
	LIGHT_P_BUFFER		= 0
};

struct ShaderData
{
	GLuint shader;

	std::string path = "";
	uint32_t identifier;
};

class ShaderManager
{
public:
	void add(Shader, std::string path);
	GLuint create();
	GLuint link(GLuint);
	void use(GLuint);
	void use(std::string);
	void destroy(GLuint);
	void destroyAll();

	static ShaderManager& getInstance()
	{
		static ShaderManager instance;
		return instance;
	};

	GLuint getCurrentProgram();

	void set(ShaderLocation, glm::mat4);
	void set(ShaderLocation, glm::vec3);
	void set(ShaderLocation, float);
	void set(ShaderLocation, int);

	ShaderManager(ShaderManager const&)  = delete;
	void operator=(ShaderManager const&) = delete;
private:
	ShaderManager();

	std::unordered_map<std::string, GLuint> shaders;

	ShaderData vertex_shader;
	ShaderData fragment_shader;
	ShaderData geometry_shader;

	GLuint cur_shader_program = 0;
	bool created = false;

	GLuint create(Shader);
	std::string read(const std::string&);
	void compile(GLuint);
	void linkProgram(GLuint);
};