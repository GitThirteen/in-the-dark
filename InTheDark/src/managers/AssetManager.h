#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdint.h>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <GLFW/glfw3.h>


#include "../util/logger/loguru.hpp"
#include "../util/util.h"


typedef std::vector<glm::vec3> vec3v;
typedef std::vector<glm::vec2> vec2v;

enum Object
{
	Crate,
	Stone
};

struct ObjData
{
	vec3v v;//v
	vec2v uv;//vt
	vec3v n;//vn
};

class AssetManager
{
public:
	/**
	 * @brief Loads an wavefront .obj file from a specified path.
	 * 
	 * @param The path the .obj is located at
	 * @return The .obj as an ObjData struct.
	*/
	ObjData loadObj(std::string);

	void loadTexture(const char *filepath, int width, int height, int nrChannel);

	/**
	 * TODO
	*/
	void loadLevel();

	/**
	 * TODO later on
	*/
	void loadAudio();

	/**
	 * @brief Returns an already loaded object as ObjData struct
	 * 
	 * @param The object that should be returned. For possible object types, refer to the local 'Object' enum
	 * @return The object data
	*/
	ObjData getObj(Object);

	/**
	 * @brief Returns a reference to the AssetManager Singleton
	 * 
	 * @return A reference to the AssetManager
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

	std::unordered_map<Object, ObjData> objects;

	void loadAll();
};