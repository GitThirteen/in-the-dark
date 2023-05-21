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
#include "../objects/GameObject.h"
#include "../objects/LightSource.h"

typedef std::vector<std::shared_ptr<LightSource>> Lights;

static std::unordered_map<Object, std::string> OBJ_PATHS = {
	{ Object::STONE,	"../_assets/objects/stone" },
	{ Object::CRATE,	"../_assets/objects/wooden_crate" },
	{ Object::TORCH,	"../_assets/objects/torch" },
	{ Object::TREASURE, "../_assets/objects/treasure_chest" }
};

struct LevelWrapper
{
	Lights lights;
	// LevelData level;
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

	Lights getLights();

	// TODO: Other getters

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
	ObjData loadObj(const std::string&);

	/**
	 * @brief Loads a .jpg file from a specified file path.
	*/
	Texture loadTexture(const std::string&);

	/**
	 * @brief TODO
	*/
	LevelWrapper loadLevel();

	/**
	 * @brief TODO
	*/
	void loadAudio();
};