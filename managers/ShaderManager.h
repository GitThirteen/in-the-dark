#pragma once
#include <string>
#include <GL/glew.h>
#include <fstream>

#include "../util/util.h"

enum Shader
{
	Vertex,
	Fragment
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