#pragma once

#include <GL/glew.h>
#include "../managers/ShaderManager.h"

typedef std::vector<glm::vec3> vec3v;
typedef std::vector<glm::vec2> vec2v;

enum Object
{
	CRATE,
	STONE
};

struct ObjData
{
	vec3v v;
	vec2v uv;
	vec3v n;

	bool incomplete()
	{
		return this->v.empty() || this->uv.empty() || this->n.empty();
	}

	int getTotalSize()
	{
		return v.size() + uv.size() + n.size();
	}
};

// TODO: This class is the C++ side of a GameObject containing the draw method.
// To make this work in Lua, we will need some sort of wrapper between a C++ GameObject
// and a Lua GameObject.
class GameObject
{
public:
	ObjData data;

	void create()
	{
		if (this->data.incomplete())
		{
			LOG_F(ERROR, "GameObject data incomplete, GameObject has not been created.");
			return;
		}

		// Create VAO and bind it
		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		this->vao = vao;

		// Our vertices, normals and texture coords are (hopefully) already indexed, so
		// all we need is 3 vbos we bind the indices to
		GLuint vbo[3];
		glGenBuffers(3, vbo);

		// Vertices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * this->data.v.size() * sizeof(GLfloat), &this->data.v, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderLocation::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(0);

		// Texture Coordinates
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * this->data.uv.size() * sizeof(GLfloat), &this->data.uv, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderLocation::UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(0);

		// Normals
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * this->data.n.size() * sizeof(GLfloat), &this->data.n, GL_STATIC_DRAW);
		glVertexAttribPointer(ShaderLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(0);

		// Unbind VAO & VBOs
		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // probably unnecessary, but just in case
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void draw()
	{
		// TODO bind translation, color, reflection, alpha, texture (if needed)

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, this->data.getTotalSize(), GL_UNSIGNED_SHORT, (void*)0);
		glBindVertexArray(0);
	}
private:
	GLuint vao = 0;
};