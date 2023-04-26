#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <glm/detail/type_vec.hpp>
#include <stdint.h>
#include <unordered_map>

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
	vec3v v;
	vec2v uv;
	vec3v n;
};

class AssetManager
{
public:
	ObjData loadObj(std::string);
	void loadTexture();
	void loadLevel();
	void loadAudio();

	ObjData getObj(Object);

	static AssetManager& getInstance()
	{
		static AssetManager instance;
		return instance;
	};

	AssetManager(AssetManager const&) = delete;
	void operator=(AssetManager const&) = delete;
private:
	AssetManager();

	std::unordered_map<Object, ObjData> objects;

	void loadAll();
};