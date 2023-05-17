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

	GameObject torch = createObj(Object::TORCH);
	this->objects.insert({ Object::TORCH, torch });

	GameObject treasure = createObj(Object::TREASURE);
	this->objects.insert({ Object::TREASURE, treasure });

	LevelWrapper level_one = loadLevel();
	this->levels.push_back(level_one);
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
	vec3v v_defs;
	vec2v uv_defs;
	vec3v n_defs;
	vec3v v_indices;

	std::ifstream file(path, std::ios::in);
	if (!file) // operator! is overloaded, see https://cplusplus.com/reference/ios/ios/operator_not/
	{
		LOG_F(ERROR, "Unable to open object file specified at path %s.", path.c_str());
		return { };
	}

	// Part 1: Parse all relevant data from the .OBJ

	std::string cur_line;
	while (std::getline(file, cur_line))
	{
		std::string data_type = cur_line.substr(0, cur_line.find(' '));
		std::string data_info = cur_line.substr(data_type.length() + 1, cur_line.length() - data_type.length() + 1);
		auto frags = util::split(data_info);

		if (data_type == "v")
		{
			auto vertex = util::floatify<glm::vec3>(frags);
			v_defs.push_back(vertex);
		}
		else if (data_type == "vt")
		{
			auto uv = util::floatify<glm::vec2>(frags);
			uv_defs.push_back(uv);
		}
		else if (data_type == "vn")
		{
			auto normal = util::floatify<glm::vec3>(frags);
			n_defs.push_back(normal);
		}
		else if (data_type == "f")
		{
			for (auto& frag : frags)
			{
				auto face = util::floatify<glm::vec3>(util::split(frag, "/"));
				v_indices.push_back(face);
			}
		}
	}

	file.close();

	// Part 2: Create a single container defining all three attributes, one vertex per vector entry [(v, uv, n), ...]

	// Note: This is a fairly primitive approach, but I've spent the whole day figuring out how to
	// create index buffers via an algorithm and I'd rather not spend more time on this
	std::vector<ObjVertex> vertices{ };
	for (auto& v_i : v_indices)
	{
		ObjVertex vertex{ };
		vertex.v = v_defs[v_i.x - 1];
		vertex.uv = uv_defs[v_i.y - 1];
		vertex.n = n_defs[v_i.z - 1];

		vertices.push_back(vertex);
	}

	ObjData result{ };
	for (int i = 0; i < vertices.size(); i++)
	{
		result.indices.push_back(i);
	}
	result.vertices = vertices;

	return result;
}

void AssetManager::loadTexture()
{
	// TODO
}

LevelWrapper AssetManager::loadLevel()
{
	// This is just mock code, please replace with actual level loading!
	
	auto p1 = std::make_shared<PointLight>();
	p1->setColor(255, 255, 255);
	p1->setPosition(-2, -5, -5);
	p1->setAttenuation(glm::vec3(0.1, 0.7, 0.3));
	
	auto l1 = std::make_shared<DirectionalLight>();
	l1->setColor(204, 204, 204);
	l1->setDirection(glm::vec3(0.0, -1.0, -1.0));

	Lights temp;
	temp.push_back(p1);
	temp.push_back(l1);

	return {
		temp
	};
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

Lights AssetManager::getLights(/* will probably need level distinguisher */)
{
	// This is just mock code, please replace!
	return this->levels[0].lights;
}