#pragma once

#include <GL/glew.h>
#include "../managers/ShaderManager.h"

typedef std::vector<glm::vec3> vec3v;
typedef std::vector<glm::vec2> vec2v;

enum Object
{
	CRATE,
	STONE,
	TORCH,
	TREASURE
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
		ShaderManager& shader = ShaderManager::getInstance();

		// TODO: Bind color and texture (if needed)
		
		// Technically we should have a separate method that only re-sets the uniforms if needed, but considering
		// that it's not particularly expensive to do set a uniform, it's not worth the hassle at the moment

		shader.set(ShaderLocation::TRANSFORM_MAT, this->trans_mat);
		shader.set(ShaderLocation::REFLECTION, this->reflection);
		shader.set(ShaderLocation::GLOSSINESS, this->glossiness);

		glBindVertexArray(this->vao);
		glDrawElements(GL_TRIANGLES, this->data.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glBindVertexArray(0);
	}

	void translate(glm::vec3 translate)
	{
		this->trans_mat = glm::translate(this->trans_mat, translate);
	}

	void rotate(glm::vec3 rotate, float angle)
	{
		this->trans_mat = glm::rotate(this->trans_mat, glm::radians(angle), rotate);
	}

	void scale(glm::vec3 scale)
	{
		this->trans_mat = glm::scale(this->trans_mat, scale);
	}

	void illuminate(glm::vec3 reflection)
	{
		this->reflection = reflection;
	}

	void illuminate(uint8_t glossiness)
	{
		this->glossiness = glossiness;
	}

	void illuminate(glm::vec3 reflection, uint8_t glossiness)
	{
		this->reflection = reflection;
		this->glossiness = glossiness;
	}
private:
	GLuint vao = 0;
	glm::mat4 trans_mat = glm::mat4(1.0);
	glm::vec3 reflection = glm::vec3(1.0, 0.4, 0.1);
	uint8_t glossiness = 8;
};