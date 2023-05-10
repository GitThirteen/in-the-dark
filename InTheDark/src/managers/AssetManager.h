#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <unordered_map>

#include "../util/logger/loguru.hpp"
#include "../util/util.h"
#include "../objects/GameObject.h"
#include "stb_image.h"

static std::unordered_map<Object, std::string> OBJ_PATHS = {
	{ Object::STONE,	"../_assets/objects/stone_tri.obj" },
	{ Object::CRATE,	"../_assets/objects/wooden_crate_tri.obj" },
	{ Object::TORCH,	"../_assets/objects/torch_tri.obj" },
	{ Object::TREASURE, "../_assets/objects/treasure_chest_tri.obj" }
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

	// TODO: Other getters

	/**
	 * @brief Returns a reference to the AssetManager Singleton.
	 * 
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

	void loadAll();

	/**
	 * @brief Optional builder method for creating a GameObject. Calls all required methods in the right order to avoid errors.
	*/
	GameObject createObj(Object);

	/**
	 * @brief Loads a wavefront .obj file from a specified file path. This method will fail if the .obj is not triangulated.
	*/
	ObjData loadObj(std::string);

	/**
	 * @brief TODO
	*/
	void loadTexture(const char* filename, int width, int height, int nrChannels);

	/**
	 * @brief TODO
	*/
	void loadLevel();

	/**
	 * @brief TODO
	*/
	void loadAudio();
};