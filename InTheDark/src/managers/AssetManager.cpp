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

	LevelWrapper level_one = loadLevel(LVL_PATHS[0]);
	this->levels.push_back(level_one);
}

GameObject AssetManager::createObj(Object specifier)
{
	GameObject obj{ };
	obj.data = loadObj(util::findPath(OBJ_PATHS.at(specifier), ".obj"));
	obj.texture = loadTexture(util::findPath(OBJ_PATHS.at(specifier), ".jpg"));
	obj.create();

	return obj;
}

obj::Data AssetManager::loadObj(const std::string& path)
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
	std::vector<obj::Vertex> vertices{ };
	for (auto& v_i : v_indices)
	{
		obj::Vertex vertex{ };
		vertex.v = v_defs[v_i.x - 1];
		vertex.uv = uv_defs[v_i.y - 1];
		vertex.n = n_defs[v_i.z - 1];

		vertices.push_back(vertex);
	}

	obj::Data result{ };
	for (int i = 0; i < vertices.size(); i++)
	{
		result.indices.push_back(i);
	}
	result.vertices = vertices;

	return result;
}

obj::Texture AssetManager::loadTexture(const std::string& path)
{
	if (path.empty())
	{
		LOG_F(ERROR, "Empty texture file path. Make sure that every asset has a .jpg or .png texture.");
		return { };
	}

	int width, height, channels;
	uint8_t* img = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (img == NULL)
	{
		LOG_F(ERROR, "Couldn't read texture for file path %s.", path.c_str());
		return { };
	}

	obj::Texture texture;
	texture.width = width;
	texture.height = height;
	texture.channels = channels;
	texture.data = img;

	return texture;
}

LevelWrapper AssetManager::loadLevel(const std::string& path)
{
	std::ifstream file(path, std::ios::in);
	if (!file)
	{
		LOG_F(ERROR, "Unable to open level file specified at path %s.", path.c_str());
		return { };
	}

	nlohmann::json level_json = nlohmann::json::parse(file);

	auto& lights = level_json["lights"];

	// This is just mock code, please replace with actual light loading!

	auto p1 = std::make_shared<PointLight>();
	p1->setColor(255, 255, 255);
	p1->setPosition(-0.2, 4, 0.5);
	p1->setAttenuation(glm::vec3(1.0, 0.027, 0.0028));

	auto l1 = std::make_shared<DirectionalLight>();
	l1->setColor(230, 230, 230);
	l1->setDirection(glm::vec3(1.0, -1.0, -1.0));

	Lights temp;
	temp.push_back(p1);
	temp.push_back(l1);

	auto& player = level_json["player"];

	// TODO

	auto& level	= level_json["level"];
	GameObjects game_objects;

	const auto saveObject = [&](obj::Container& c) -> void
	{
		auto obj = std::make_shared<GameObject>(this->getObj(c.type));
		obj->translate(c.position);
		obj->illuminate(c.reflection, c.glossiness);
		game_objects.push_back(obj);
	};

	for (auto& layer : level["data"])
	{
		for (auto& game_obj : layer)
		{
			auto obj_data = game_obj.get<obj::Container>();
			saveObject(obj_data);
			
			if (!obj_data.hasChildren()) continue;

			for (auto& child : obj_data.flatten().children)
			{
				saveObject(child);
			}
		}
	}

	return {
		temp,
		game_objects
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

LevelWrapper AssetManager::getLevel(uint16_t level_number)
{
	if (level_number < 0 || level_number > 65535)
	{
		LOG_F(ERROR, "Error attempting to obtain level. Parameter 'level_number' cannot be < 0 or > 65535.");
		return { };
	}

	return this->levels[level_number];
}