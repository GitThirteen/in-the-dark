#pragma once

#include <GL/glew.h>
#include "../managers/ShaderManager.h"

typedef std::vector<glm::vec3> vec3v;
typedef std::vector<glm::vec2> vec2v;

static uint16_t TEX_UNIT = -1;

enum Object
{
	CRATE,
	STONE,
	TORCH,
	TREASURE
};

namespace obj
{
	struct Container
	{
		Object type;
		glm::vec3 position;
		glm::vec3 reflection;
		uint8_t glossiness;
	};

	// ObjContainer JSON mapping functions
	inline void to_json(nlohmann::json& j, const obj::Container& c)
	{
		// This will never happen but the json lib requires it.
		// If you should ever need it, blame past you @future me
	}

	inline void from_json(const nlohmann::json& j, obj::Container& c)
	{
		j.at("type").get_to(c.type);
		auto& pos = j.at("position");
		c.position = glm::vec3(pos[0], pos[1], pos[2]);
		auto& ref = j.at("reflection");
		c.reflection = glm::vec3(ref[0], ref[1], ref[2]);
		j.at("glossiness").get_to(c.glossiness);
	}

	struct Vertex
	{
		glm::vec3 v;
		glm::vec2 uv;
		glm::vec3 n;
	};

	struct Data
	{
		GLuint vao = 0;

		std::vector<GLushort> indices;
		std::vector<obj::Vertex> vertices;

		bool incomplete()
		{
			if (indices.empty() || indices.size() != vertices.size()) return true;
			for (auto& vertex : this->vertices)
			{
				if (vertex.v.length() != 3 || vertex.uv.length() != 2 || vertex.n.length() != 3) return true;
			}
			return false;
		}

		void create()
		{
			if (incomplete())
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
			glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * (2 * sizeof(glm::vec3) + sizeof(glm::vec2)), this->vertices.data(), GL_STATIC_DRAW);

			auto stride = 8 * sizeof(GLfloat);

			glVertexAttribPointer(ShaderLocation::POSITION, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
			glEnableVertexAttribArray(ShaderLocation::POSITION);

			glVertexAttribPointer(ShaderLocation::UV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(ShaderLocation::UV);

			glVertexAttribPointer(ShaderLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(5 * sizeof(GLfloat)));
			glEnableVertexAttribArray(ShaderLocation::NORMAL);

			// Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLushort), this->indices.data(), GL_STATIC_DRAW);

			/* ---- Unbind VAO & VBOs ---- */

			glBindVertexArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	};

	struct Texture
	{
		GLuint id = 0;
		uint16_t tex_unit = 0;

		int width = 0;
		int height = 0;
		int channels = 0;
		uint8_t* data = 0;

		void create()
		{
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				// use linear for magnifying
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// use linear blend for minifying
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
			glGenerateMipmap(GL_TEXTURE_2D);

			this->id = texture;
			this->tex_unit = ++TEX_UNIT;
		}

		void bind()
		{
			glActiveTexture(GL_TEXTURE0 + tex_unit);
			glBindTexture(GL_TEXTURE_2D, this->id);
		}

		void unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};
}

// TODO: This class is the C++ side of a GameObject containing the draw method.
// To make this work in Lua, we will need some sort of mapper between a C++ GameObject
// and a Lua GameObject.
class GameObject
{
public:
	obj::Data data;
	obj::Texture texture;

	void create()
	{
		this->data.create();
		this->texture.create();
	}

	void draw()
	{
		ShaderManager& shader = ShaderManager::getInstance();
		
		// Technically we should have a separate method that only re-sets the uniforms if needed, but considering
		// that it's not particularly expensive to do set a uniform, it's not worth the hassle at the moment

		shader.set(ShaderLocation::TRANSFORM_MAT, this->trans_mat);
		shader.set(ShaderLocation::REFLECTION, this->reflection);
		shader.set(ShaderLocation::GLOSSINESS, this->glossiness);

		bool tex_loaded = this->texture.data != NULL;
		if (tex_loaded)
		{
			shader.set(ShaderLocation::TEXTURE, this->texture.tex_unit);
			this->texture.bind();
		}

		glBindVertexArray(this->data.vao);
		glDrawElements(GL_TRIANGLES, this->data.indices.size(), GL_UNSIGNED_SHORT, (void*)0);
		glBindVertexArray(0);

		if (tex_loaded)
		{
			this->texture.unbind();
		}
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
	glm::mat4 trans_mat = glm::mat4(1.0);
	glm::vec3 reflection = glm::vec3(1.0, 0.4, 0.1);
	uint8_t glossiness = 8;
};