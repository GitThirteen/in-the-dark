#include "AssetManager.h"

AssetManager::AssetManager()
{
	this->assets = std::unordered_map<AssetType, GameAsset>();
	loadAll();
}

void AssetManager::loadAll()
{
	for (int asset_type = AssetType::CRATE; asset_type < AssetType::UNDEFINED; asset_type++)
	{
		AssetType type = static_cast<AssetType>(asset_type);
		GameAsset asset = createAsset(type);

		this->assets.insert({ type, asset });
	}

	LevelWrapper level_one = loadLevel(LVL_PATHS[0]);
	this->levels.push_back(level_one);
}

GameAsset AssetManager::createAsset(AssetType specifier)
{
	GameAsset obj{ };
	obj.data = loadObj(util::findPath(ASSET_PATHS.at(specifier), ".obj"));
	obj.texture = loadTexture(util::findPath(ASSET_PATHS.at(specifier), ".jpg"));
	obj.create();

	return obj;
}

asset::Data AssetManager::loadObj(const std::string& path)
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
	std::vector<asset::Vertex> vertices{ };
	for (auto& v_i : v_indices)
	{
		asset::Vertex vertex{ };
		vertex.v = v_defs[v_i.x - 1];
		vertex.uv = uv_defs[v_i.y - 1];
		vertex.n = n_defs[v_i.z - 1];

		vertices.push_back(vertex);
	}

	asset::Data result{ };
	for (int i = 0; i < vertices.size(); i++)
	{
		result.indices.push_back(i);
	}
	result.vertices = vertices;

	return result;
}

asset::Texture AssetManager::loadTexture(const std::string& path)
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

	asset::Texture texture;
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

	// Extract lights from JSON

	auto& lights = level_json["lights"];
	LightWrapper game_lights;

	auto& dir_j = lights["directional"];
	auto dir_light = dir_j.get<lightSource::Directional>();
	game_lights.directionalLight = dir_light;

	for (auto& pnt_j : lights["point"])
	{
		auto pnt_light = pnt_j.get<lightSource::Point>();
		game_lights.pointLights.entities.push_back(pnt_light);
	}
	game_lights.pointLights.create();

	/* ---- */

	const auto convertToObj = [&](asset::JSONContainer& c) -> std::shared_ptr<GameObject>
	{
		auto obj = c.type == AssetType::PLAYER
			? std::make_shared<Player>() 
			: std::make_shared<GameObject>();

		obj->asset = getAsset(c.type);
		obj->asset.translate(c.position);
		obj->asset.illuminate(c.reflection, c.glossiness);

		obj->position = c.position;
		obj->bbox = c.bbox;

		return obj;
	};

	/* ---- */

	// Extract player from JSON

	auto& player = level_json["player"];
	player["type"] = AssetType::PLAYER;
	auto data = player.get<asset::JSONContainer>();
	std::shared_ptr<Player> game_character = std::dynamic_pointer_cast<Player>(convertToObj(data));

	assert(game_character != nullptr);

	// Extract objects from JSON

	auto& level = level_json["level"];
	GameObjects game_objects;

	game_objects.push_back(game_character); // So we don't have to handle the player separately in the draw logic (it's the same pointer)

	for (auto& layer : level["data"])
	{
		for (auto& game_obj : layer)
		{
			auto obj_data = game_obj.get<asset::JSONContainer>();
			game_objects.push_back(convertToObj(obj_data));
			
			if (!obj_data.hasChildren()) continue;

			for (auto& child : obj_data.flatten().children)
			{
				game_objects.push_back(convertToObj(child));
			}
		}
	}

	return {
		game_lights,
		game_character,
		game_objects
	};
}

void AssetManager::loadAudio()
{
	// TODO
}

/* ---- GETTER ---- */

GameAsset AssetManager::getAsset(AssetType identifier)
{
	try
	{
		return this->assets.at(identifier);
	}
	catch (std::out_of_range e)
	{
		LOG_F(ERROR, "Error attempting to obtain object.");
		return { };
	}
}

LevelWrapper AssetManager::getLevel(uint16_t level_number)
{
	if (level_number < 0 || level_number > 65535 || level_number >= this->levels.size())
	{
		LOG_F(ERROR, "Error attempting to obtain level.");
		return { };
	}

	return this->levels[level_number];
}