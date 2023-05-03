#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <unordered_map>

#include "../util/logger/loguru.hpp"
#include "../util/util.h"
#include "../objects/GameObject.h"

typedef std::vector<glm::vec3> vec3v;
typedef std::vector<glm::vec2> vec2v;

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
	 * Loads a wavefront .obj file from a specified file path. This method will fail if the .obj is not triangulated.
	*/
	ObjData loadObj(std::string);

	/**
	 * TODO
	*/
	void loadTexture();

	/**
	 * TODO
	*/
	void loadLevel();

	/**
	 * TODO
	*/
	void loadAudio();
};