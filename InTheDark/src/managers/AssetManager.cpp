#include "AssetManager.h"

AssetManager::AssetManager()
{
	this->objects = std::unordered_map<Object, GameObject>();
	loadAll();
}

void AssetManager::loadAll()
{
	GameObject stone = createObj(Object::STONE);
	this->objects.insert({ Object::STONE, stone });

	GameObject crate = createObj(Object::CRATE);
	this->objects.insert({ Object::CRATE, crate });
}

GameObject AssetManager::createObj(Object specifier)
{
	GameObject obj{ };
	obj.data = loadObj(OBJ_PATHS.at(specifier));
	obj.create();

	return obj;
}

ObjData AssetManager::loadObj(std::string path)
{
	ObjData data{ };
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
			auto vertex = util::floatify<glm::vec3>(frags);
			data.v.push_back(vertex);
		}
		else if (data_type == "vt")
		{
			auto uv = util::floatify<glm::vec2>(frags);
			data.uv.push_back(uv);
		}
		else if (data_type == "vn")
		{
			auto normal = util::floatify<glm::vec3>(frags);
			data.n.push_back(normal);
		}
		else if (data_type == "f")
		{
			for (auto& frag : frags)
			{
				auto face = util::floatify<glm::vec3>(util::split(frag, "/"));

				data.i.push_back(face.x - 1);
				// Consider pushing y and z back too if we want to further optimize this
			}
		}
	}

	file.close();

	return data;
}

void AssetManager::loadTexture()
{
	// TODO
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

GameObject AssetManager::getObj(Object object_type)
{
	try
	{
		return this->objects.at(object_type);
	}
	catch (std::out_of_range e)
	{
		LOG_F(ERROR, "Error attempting to obtain object.");
		return { };
	}
}