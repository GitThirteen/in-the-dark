#include "ShaderManager.h"

ShaderManager::ShaderManager() { }

void ShaderManager::add(Shader shader_type, std::string path)
{
	switch (shader_type)
	{
		case Shader::Vertex:
			this->vert_sh_path = path;
			break;
		case Shader::Fragment:
			this->frag_sh_path = path;
			break;
	}
}

void ShaderManager::create()
{
	if (this->vert_sh_path.empty() || this->frag_sh_path.empty())
	{
		util::LOG_ERROR_AND_EXIT("ShaderManager is missing shader(s) to create shader program. Please verify if shader paths for vert and frag have been set.");
	}

	/* ---- Shader Init ---- */

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	auto vertContent = this->read(this->vert_sh_path);
	auto fragContent = this->read(this->frag_sh_path);

	auto* vert = (const GLchar*)vertContent.c_str();
	glShaderSource(vertShader, 1, &vert, NULL);
	this->compile(vertShader);

	auto* frag = (const GLchar*)fragContent.c_str();
	glShaderSource(fragShader, 1, &frag, NULL);
	this->compile(fragShader);

	/* ---- Program Init ---- */

	GLuint program = glCreateProgram();

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	this->linkProgram(program);

	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	this->shader = program;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

void ShaderManager::destroy()
{
	glDeleteProgram(this->shader);
}

void ShaderManager::set(ShaderLocation location, glm::mat4 value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderManager::set(ShaderLocation location, glm::vec3 value)
{
	glUniform3fv(location, 1, glm::value_ptr(value));
}

std::string ShaderManager::read(const std::string& path)
{
	std::string content;
	std::ifstream file(path, std::ios::in);
	
	if (!file)
	{
		util::LOG_ERROR_AND_EXIT("Couldn't read file from filepath '" + path + "'.");
	}

	std::string cur_line;
	while (std::getline(file, cur_line))
	{
		content.append(cur_line + "\n");
	}
	file.close();

	return content;
}

void ShaderManager::compile(GLuint shader)
{
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		glDeleteShader(shader);
		util::LOG_ERROR_AND_EXIT("Shader initialization failed.");
	}
}

void ShaderManager::linkProgram(GLuint program)
{
	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE) {
		glDeleteProgram(program);
		util::LOG_ERROR_AND_EXIT("Program initialization failed.");
	}
}