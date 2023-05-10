#include "AssetManager.h"
#include "stb_image.h"

AssetManager::AssetManager()
{
	this->objects = std::unordered_map<Object, ObjData>();
	loadAll();
}

void AssetManager::loadAll()
{
	ObjData stone = loadObj("../_assets/objects/stone_tri.obj");
	this->objects.insert({ Object::Stone, stone });
}

ObjData AssetManager::loadObj(std::string path)
{
	ObjData data;
	vec3v indices_v;

	std::ifstream file(path, std::ios::in);
	if (!file) // operator! is overloaded, see https://cplusplus.com/reference/ios/ios/operator_not/
	{
		LOG_F(ERROR, "Unable to open object file specified at path %s.", path.c_str());
		return { };
	}

	std::string cur_line;
	while (std::getline(file, cur_line))
	{
		std::string data_type = cur_line.substr(0, cur_line.find(' '));
		std::string data_info = cur_line.substr(data_type.length() + 1, cur_line.length() - data_type.length() + 1);
		auto frags = util::split(data_info);

		if (data_type == "v")
		{
			auto v = util::floatify<glm::vec3>(frags);
			data.v.push_back(v);
		}
		else if (data_type == "vt")
		{
			auto uv = util::floatify<glm::vec2>(frags);
			data.uv.push_back(uv);
		}
		else if (data_type == "vn")
		{
			auto nl = util::floatify<glm::vec3>(frags);
			data.n.push_back(n);
		}
		else if (data_type == "f")
		{
			for (auto& frag : frags)
			{
				auto face = util::floatify<glm::vec3>(util::split(frag, "/"));
				indices_v.push_back(face);
			}
		}
	}

	file.close();

	/* ---- Indexing ---- */

	ObjData result;

	for (auto& indices : indices_v)
	{
		glm::vec3 vertex = data.v[indices.x - 1];
		result.v.push_back(v);

		glm::vec2 uv = data.uv[indices.y - 1];
		result.uv.push_back(uv);

		glm::vec3 normal = data.n[indices.z - 1];
		result.n.push_back(n);
	}

	return result;
}

void AssetManager::loadTexture(const char *filepath, int width, int height, int nrChannel)
{
	unsigned int texture; //maybe we need a texture id that we pass on with the parameters
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannel, 0); 

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

}

void AssetManager::loadLevel()
{
	// TODO
}

void AssetManager::loadAudio()
{
	// TODO
}

/* ---- GETTER ---- */

ObjData AssetManager::getObj(Object object_type)
{
	return this->objects.at(object_type);
}