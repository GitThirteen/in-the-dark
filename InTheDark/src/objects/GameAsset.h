#pragma once

#include <GL/glew.h>
#include "../util/json.hpp"
#include "../managers/ShaderManager.h"
#include "BBox.h"

typedef std::vector<glm::vec3> vec3v;
typedef std::vector<glm::vec2> vec2v;

enum AssetType
{
	CRATE,
	STONE,
	SAND,
	TORCH,
	TREASURE,
	PLAYER,
	SMOKE,
	SPIKES,
	STONE_GOLD,
	STONE_RED,
	UNDEFINED // we only use this one for iterating, it doesn't actually exist, do NOT use it for the love of god
};

namespace asset
{
	struct JSONContainer
	{
		AssetType type;
		glm::vec3 position;
		glm::vec3 reflection;
		glm::vec3 rot_axis = glm::vec3(0.0f);
		float rot_deg = 0.0f;
		uint8_t glossiness;
		BBox bbox;
		std::vector<asset::JSONContainer> children;

		/**
		* @brief Returns true if this container has children, false otherwise.
		*/
		bool hasChildren()
		{
			return !this->children.empty();
		};

		/**
		* @brief Flattens the children of this container to a single layer.
		* Each children has its absolute world position applied to it.
		*
		* @return A container with a flattened children list.
		*/
		asset::JSONContainer flatten()
		{
			std::vector<asset::JSONContainer> payload;

			if (hasChildren())
			{
				this->children = extractChildren(*this, payload);
			}

			return *this;
		};

	private:
		std::vector<asset::JSONContainer> extractChildren(asset::JSONContainer& current, std::vector<asset::JSONContainer>& payload)
		{
			for (auto& child : current.children)
			{
				child.position += current.position;
				payload.push_back(child);

				if (child.hasChildren())
				{
					extractChildren(child, payload);
				}
			}

			return payload;
		};
	};

	// ObjContainer JSON mapping functions

	// This will never happen but the json lib requires it.
	// If you should ever need it, blame past you @future me
	inline void to_json(nlohmann::json& j, const asset::JSONContainer& c) = delete;

	inline void from_json(const nlohmann::json& j, asset::JSONContainer& c)
	{
		j.at("type").get_to(c.type);
		auto& pos = j.at("position");
		c.position = glm::vec3(pos[0], pos[1], pos[2]);
		auto& ref = j.at("reflection");
		c.reflection = glm::vec3(ref[0], ref[1], ref[2]);
		j.at("glossiness").get_to(c.glossiness);

		auto rotation = j.find("rotation");
		if (rotation != j.end())
		{
			auto& axis = rotation.value().at("axis");
			c.rot_axis = glm::vec3(axis[0], axis[1], axis[2]);
			c.rot_deg = rotation.value().at("deg");
		}

		auto& lower = j.at("bbox")["lower"];
		auto& upper = j.at("bbox")["upper"];
		c.bbox.lower = glm::vec3(lower[0], lower[1], lower[2]);
		c.bbox.upper = glm::vec3(upper[0], upper[1], upper[2]);

		auto& children = j.find("children");
		if (children == j.end()) return;

		asset::JSONContainer container;

		for (auto& child : children.value())
		{
			from_json(child, container);
		}

		c.children.push_back(std::move(container));
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
		std::vector<asset::Vertex> vertices;

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

			glVertexAttribPointer(ShaderLocation::Default::POSITION, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
			glEnableVertexAttribArray(ShaderLocation::Default::POSITION);

			glVertexAttribPointer(ShaderLocation::Default::UV, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(ShaderLocation::Default::UV);

			glVertexAttribPointer(ShaderLocation::Default::NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(5 * sizeof(GLfloat)));
			glEnableVertexAttribArray(ShaderLocation::Default::NORMAL);

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

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
			glGenerateMipmap(GL_TEXTURE_2D);

			this->id = texture;
			this->tex_unit = ++global_tex_counter;

			unbind();
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

	private:
		inline static int global_tex_counter = -1;
	};
}

class GameAsset
{
public:
	AssetType type;
	asset::Data data;
	asset::Texture texture;

	void create()
	{
		this->data.create();
		this->texture.create();
	}

	void draw()
	{
		ShaderManager& shader = ShaderManager::getInstance();
		shader.use("shader");
		
		// Technically we should have a separate method that only re-sets the uniforms if needed, but considering
		// that it's not particularly expensive to do set a uniform, it's not worth the hassle at the moment

		shader.set(ShaderLocation::Default::TRANSFORM_MAT, this->trans_mat);
		shader.set(ShaderLocation::Default::REFLECTION, this->reflection);
		shader.set(ShaderLocation::Default::GLOSSINESS, this->glossiness);

		bool tex_loaded = this->texture.data != NULL;
		if (tex_loaded)
		{
			shader.set(ShaderLocation::Default::TEXTURE, this->texture.tex_unit);
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