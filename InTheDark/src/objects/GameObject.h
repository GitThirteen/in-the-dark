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

struct ObjVertex
{
	glm::vec3 v;
	glm::vec2 uv;
	glm::vec3 n;
};

struct ObjData
{
	std::vector<GLushort> indices;
	std::vector<ObjVertex> vertices;

	bool incomplete()
	{
		if (indices.empty() || indices.size() != vertices.size()) return true;
		for (auto& vertex : this->vertices)
		{
			if (vertex.v.length() != 3 || vertex.uv.length() != 2 || vertex.n.length() != 3) return true;
		}
		return false;
	}
};

// TODO: This class is the C++ side of a GameObject containing the draw method.
// To make this work in Lua, we will need some sort of mapper between a C++ GameObject
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

		/* ---- Create VAO and bind it ---- */

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		this->vao = vao;

		/* ---- Create VBO and bind our vertices, indices, texture coordinates and normals to them ---- */

		GLuint vbo[2];
		glGenBuffers(2, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * (2 * sizeof(glm::vec3) + sizeof(glm::vec2)), data.vertices.data(), GL_STATIC_DRAW);

		auto stride = 8 * sizeof(GLfloat);

		glVertexAttribPointer(ShaderLocation::POSITION, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
		glEnableVertexAttribArray(ShaderLocation::POSITION);

		glVertexAttribPointer(ShaderLocation::UV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(ShaderLocation::UV);

		glVertexAttribPointer(ShaderLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(ShaderLocation::NORMAL);

		/*
		// Vertices
		auto& vertices = this->data.v;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(ShaderLocation::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(ShaderLocation::POSITION);

		std::cout << std::to_string(vertices.size()) + "; sizeof: " + std::to_string(sizeof(glm::vec3)) << std::endl;

		// Texture Coordinates
		auto& tex_coords = this->data.uv;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(glm::vec2), tex_coords.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(ShaderLocation::UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(ShaderLocation::UV);

		// Normals
		auto& normals = this->data.n;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(ShaderLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(ShaderLocation::NORMAL);*/

		// Indices
		auto& indices = this->data.indices;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);

		/* ---- Unbind VAO & VBOs ---- */

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void draw()
	{
		// TODO: Bind translation, color, reflection, alpha, texture (if needed)

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, this->data.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glBindVertexArray(0);
	}
private:
	GLuint vao = 0;
};