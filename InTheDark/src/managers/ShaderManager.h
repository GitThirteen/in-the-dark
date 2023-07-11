#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

#include "../util/util.h"

enum class Shader
{
	Vertex,
	Fragment,
	Geometry
};

// This monstrosity was sponsored by the C++ committee's decision that enum classes shouldn't have implicit enum decay
namespace ShaderLocation {
	namespace Default {
		enum Values {
			POSITION			= 0,
			NORMAL				= 1,
			UV					= 2,
			TRANSFORM_MAT		= 3,
			VIEWPROJECTION_MAT	= 4,
			LIGHT_D_DIRECTION	= 5,
			LIGHT_D_COL			= 6,
			CAMERA_POSITION		= 7,
			REFLECTION			= 8,
			GLOSSINESS			= 9,
			TEXTURE				= 10
		};

		enum Bindings {
			LIGHT_P_BUFFER		= 0
		};
	}

	namespace PProc {
		enum Values {
			POSITION			= 0,
			UV					= 1,
			SCREEN_WIDTH		= 2,
			SCREEN_HEIGHT		= 3,
			COLOR_TEXTURE		= 4,
			DEPTH_TEXTURE		= 5
		};
	}

	namespace Particle {
		enum Values {
			POSITION			= 0,
			VELOCITY			= 1,
			AGE					= 2,
			COLOR				= 3,
			DELTA_TIME			= 4,
			LIFETIME			= 5,
			SYSTEM_POSITION		= 6,
			GENERATION_FLAG		= 7
		};
	}

	namespace Billboard {
		enum Values {
			POSITION			= 0,
			VIEWPROJECTION_MAT	= 1,
			CAMERA_POSITION		= 2,
			PARTICLE_SIZE		= 3,
			TEXTURE				= 4
		};
	}
}

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

	void set(GLint, glm::mat4);
	void set(GLint, glm::vec3);
	void set(GLint, float);
	void set(GLint, int);

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