#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM

#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <unordered_map>

#include "../util/logger/loguru.hpp"
#include "../util/util.h"
#include "../util/stb_image.h"
#include "../util/json.hpp"
#include "../objects/GameObject.h"
#include "../objects/LightSource.h"
#include "../objects/Player.h"

typedef std::vector<std::shared_ptr<lightSource::Point>> PointLights;
typedef std::vector<std::shared_ptr<GameObject>> GameObjects;

static const std::unordered_map<Object, std::string> OBJ_PATHS = {
	{ Object::CRATE,	"../_assets/objects/wooden_crate" },
	{ Object::STONE,	"../_assets/objects/stone" },
	{ Object::TORCH,	"../_assets/objects/torch" },
	{ Object::TREASURE, "../_assets/objects/treasure_chest" }
};

static const std::vector<std::string> LVL_PATHS = {
	"../_assets/levels/level_0.json"
};

struct LightWrapper
{
	lightSource::Directional directionalLight;
	PointLightDataHandler pointLights;
};

struct LevelWrapper
{
	LightWrapper lights;
	Player player;
	GameObjects data;
};

class AssetManager
{
public:
	/**
	 * @brief Returns an already loaded object as GameObject.
	 * 
	 * @param The object that should be returned. For possible object types, refer to the 'Object' enum in the GameObject header
	 * @return The object as GameObject. If no GameObject has been created with the specified key, an empty GameObject is returned.
	*/
	GameObject getObj(Object);

	/**
	 * @brief Returns already loaded level data (lights, player, objects) as LevelWrapper. Please notice that this method fails silently
	 * and returns an empty object { } if the uint is greater than 65535 or an attempt is made to fetch via an inexistant level number.
	 * @return All relevant level data as typeof LevelWrapper.
	 * 
	 * @param An unsigned integer representing the level number of the level to be returned.
	*/
	LevelWrapper getLevel(uint16_t);

	/**
	 * @return A reference to the AssetManager.
	*/
	static AssetManager& getInstance()
	{
		static AssetManager instance;
		return instance;
	};

	AssetManager(AssetManager const&)	= delete;
	void operator=(AssetManager const&) = delete;
private:
	AssetManager();

	std::unordered_map<Object, GameObject> objects;
	std::vector<LevelWrapper> levels;

	void loadAll();

	/**
	 * @brief Optional builder method for creating a GameObject. Calls all required methods in the right order to avoid errors.
	*/
	GameObject createObj(Object);

	/**
	 * @brief Loads a wavefront .obj file from a specified file path. This method will fail if the .obj is not triangulated.
	*/
	obj::Data loadObj(const std::string&);

	/**
	 * @brief Loads a .jpg file from a specified file path.
	*/
	obj::Texture loadTexture(const std::string&);

	/**
	 * @brief Loads a level specified in a .json file from a specified file path and returns it in a typeof LevelWrapper.
	*/
	LevelWrapper loadLevel(const std::string&);

	/**
	 * @brief TODO
	*/
	void loadAudio();
};