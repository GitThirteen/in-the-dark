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
	Fragment
};

enum ShaderLocation {
	POSITION			= 0,
	NORMAL				= 1,
	UV					= 2,
	TRANSFORM_MAT		= 3,
	VIEWPROJECTION_MAT	= 4,
	LIGHT_D_DIRECTION	= 5,
	LIGHT_D_COL			= 6,
	//LIGHT_P_POSITION	= 7,
	//LIGHT_P_COL		= 8,
	//LIGHT_P_ATTENUATION = 9,
	CAMERA_POSITION		= 10,
	REFLECTION			= 11,
	GLOSSINESS			= 12,
	TEXTURE				= 13
};

enum ShaderBinding {
	LIGHT_P_BUFFER		= 0
};

class ShaderManager
{
public:
	void add(Shader, std::string path);
	GLuint create();
	void use(GLuint);
	void use(std::string);
	void destroy(GLuint);
	void destroyAll();

	static ShaderManager& getInstance()
	{
		static ShaderManager instance;
		return instance;
	};

	void set(ShaderLocation, glm::mat4);
	void set(ShaderLocation, glm::vec3);
	void set(ShaderLocation, float);
	void set(ShaderLocation, int);

	ShaderManager(ShaderManager const&)  = delete;
	void operator=(ShaderManager const&) = delete;
private:
	ShaderManager();

	std::unordered_map<std::string, GLuint> shaders;
	std::string vert_sh_path = "";
	std::string frag_sh_path = "";

	std::string read(const std::string&);
	void compile(GLuint);
	void linkProgram(GLuint);
};