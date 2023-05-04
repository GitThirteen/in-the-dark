#pragma once
#include <string>
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
	VIEWPROJECTION_MAT	= 4
};

class ShaderManager
{
public:
	void add(Shader, std::string path);
	void create();
	void destroy();

	static ShaderManager& getInstance()
	{
		static ShaderManager instance;
		return instance;
	};

	void set(ShaderLocation, glm::mat4);
	void set(ShaderLocation, glm::vec3);

	ShaderManager(ShaderManager const&)  = delete;
	void operator=(ShaderManager const&) = delete;
private:
	ShaderManager();

	GLuint shader = 0;
	std::string vert_sh_path = "";
	std::string frag_sh_path = "";

	std::string read(const std::string&);
	void compile(GLuint);
	void linkProgram(GLuint);
};