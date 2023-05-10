#pragma once

#include <unordered_map>
#include <stdexcept>
#include <typeinfo>
#include <iostream>

#include "../util/ini.h"
#include "../util/util.h"
#include "../util/logger/loguru.hpp"

enum SType { INT, FLOAT, STRING, INVALID };

struct SValue {
	SType type;
	struct {
		int i;
		float f;
		std::string s;
	} value;
};

class SettingsManager
{
public:
	template <typename T>
	T get(std::string)
	{
		static_assert(false);
		return T();
	}

	template<>
	int get<int>(std::string name)
	{
		SValue setting = getData(name, SType::INT);

		if (setting.type == SType::INVALID) return 0;
		return setting.value.i;
	}

	template<>
	float get<float>(std::string name)
	{
		SValue setting = getData(name, SType::FLOAT);

		if (setting.type == SType::INVALID) return 0.0f;
		return setting.value.f;
	}

	template<>
	std::string get<std::string>(std::string name)
	{
		SValue setting = getData(name, SType::STRING);

		if (setting.type == SType::INVALID) return "N/A";
		return setting.value.s;
	}

	// TODO: Write & Update

	static SettingsManager& getInstance()
	{
		static SettingsManager instance;
		return instance;
	};

	SettingsManager(SettingsManager const&) = delete;
	void operator=(SettingsManager const&)	= delete;

private:
	mINI::INIFile file;
	mINI::INIStructure payload;
	std::unordered_map<std::string, SValue> settings;

	SettingsManager();
	void loadData();
	SValue getData(std::string, SType);
};