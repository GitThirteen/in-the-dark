#include "ShaderManager.h"

ShaderManager::ShaderManager() { }

void ShaderManager::add(Shader shader_type, std::string path)
{
	switch (shader_type)
	{
		case Shader::Vertex:
			this->vertex_shader.path = path;
			this->vertex_shader.identifier = GL_VERTEX_SHADER;
			break;
		case Shader::Fragment:
			this->fragment_shader.path = path;
			this->fragment_shader.identifier = GL_FRAGMENT_SHADER;
			break;
		case Shader::Geometry:
			this->geometry_shader.path = path;
			this->geometry_shader.identifier = GL_GEOMETRY_SHADER;
			break;
	}
}

GLuint ShaderManager::create()
{
	if (this->vertex_shader.path.empty() || this->fragment_shader.path.empty())
	{
		auto error_msg = "ShaderManager is missing shader(s) to create shader program. Please verify if shader paths for vert and frag have been set.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}

	GLuint program = glCreateProgram();

	auto vert = create(Shader::Vertex);
	this->vertex_shader.shader = vert;
	glAttachShader(program, vert);

	auto frag = create(Shader::Fragment);
	this->fragment_shader.shader = frag;
	glAttachShader(program, frag);

	if (!this->geometry_shader.path.empty())
	{
		auto geom = create(Shader::Geometry);
		this->geometry_shader.shader = geom;
		glAttachShader(program, geom);
	}

	this->created = true;
	return program;
}

GLuint ShaderManager::create(Shader shader_type)
{
	ShaderData data;
	if (shader_type == Shader::Vertex) data = this->vertex_shader;
	else if (shader_type == Shader::Fragment) data = this->fragment_shader;
	else if (shader_type == Shader::Geometry) data = this->geometry_shader;

	GLuint shader = glCreateShader(data.identifier);

	auto shader_content = this->read(data.path);
	auto* content = (const GLchar*)shader_content.c_str();

	glShaderSource(shader, 1, &content, NULL);
	this->compile(shader);

	return shader;
}

GLuint ShaderManager::link(GLuint program)
{
	if (!this->created)
	{
		auto error_msg = "Shader program has not been created before linkage. Please create shaders via create() before linking them.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}

	if (this->vertex_shader.path.empty() || this->fragment_shader.path.empty())
	{
		auto error_msg = "Missing shaders for linking. Please make sure you first load all required shaders and call create() before linking.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}

	this->linkProgram(program);

	glDetachShader(program, this->vertex_shader.shader);
	glDetachShader(program, this->fragment_shader.shader);
	if (!this->geometry_shader.path.empty()) glDetachShader(program, this->geometry_shader.shader);

	this->shaders.insert({ util::split(util::split(this->vertex_shader.path, "/").back(), ".")[0], program });

	glDeleteShader(this->vertex_shader.shader);
	glDeleteShader(this->fragment_shader.shader);
	if (!this->geometry_shader.path.empty()) glDeleteShader(this->geometry_shader.shader);

	this->vertex_shader.path = "";
	this->fragment_shader.path = "";
	this->geometry_shader.path = "";

	this->created = false;
	return program;
}

void ShaderManager::use(GLuint shader)
{
	glUseProgram(shader);
	this->cur_shader_program = shader;
}

void ShaderManager::use(std::string shader_name)
{
	try {
		GLuint shader = this->shaders.at(shader_name);
		this->cur_shader_program = shader;
		glUseProgram(shader);
	}
	catch (std::out_of_range e)
	{
		LOG_F(ERROR, "No shader with name %s has been found.", shader_name.c_str());
		return;
	}
}

void ShaderManager::destroy(GLuint shader)
{
	glDeleteProgram(shader);
}

void ShaderManager::destroyAll()
{
	for (auto& shader : this->shaders)
	{
		glDeleteProgram(shader.second);
	}
}

GLuint ShaderManager::getCurrentProgram()
{
	return this->cur_shader_program;
}

void ShaderManager::set(GLint location, glm::mat4 value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderManager::set(GLint location, glm::vec3 value)
{
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderManager::set(GLint location, float value)
{
	glUniform1f(location, value);
}

void ShaderManager::set(GLint location, int value)
{
	glUniform1i(location, value);
}

std::string ShaderManager::read(const std::string& path)
{
	std::string content;
	std::ifstream file(path, std::ios::in);
	
	if (!file)
	{
		auto error_msg = "Couldn't read file from filepath '" + path + "'.";
		LOG_F(ERROR, error_msg.c_str());
		exit(EXIT_FAILURE);
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
		std::vector<GLchar> errorLog(512);
		glGetShaderInfoLog(shader, 512, NULL, &errorLog[0]);
		LOG_F(ERROR, std::string(errorLog.begin(), errorLog.end()).c_str());

		glDeleteShader(shader);

		auto error_msg = "Shader initialization failed.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}
}

void ShaderManager::linkProgram(GLuint program)
{
	glLinkProgram(program);

	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&success);
	if (success == GL_FALSE) {
		std::vector<GLchar> errorLog(512);
		glGetProgramInfoLog(program, 512, NULL, &errorLog[0]);
		LOG_F(ERROR, std::string(errorLog.begin(), errorLog.end()).c_str());

		glDeleteProgram(program);

		auto error_msg = "Program initialization failed.";
		LOG_F(ERROR, error_msg);
		exit(EXIT_FAILURE);
	}
}