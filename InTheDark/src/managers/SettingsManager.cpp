#include "SettingsManager.h"

SettingsManager::SettingsManager() : file("")
{
	const std::string SETTINGS_PATH = "../_assets/settings.ini";
	this->file = mINI::INIFile(SETTINGS_PATH);

	this->payload = mINI::INIStructure();
	this->settings = std::unordered_map<std::string, SValue>();

	this->file.read(this->payload);
	loadData();
}

void SettingsManager::loadData()
{
	for (auto& category : this->payload)
	{
		const auto& [name, elements] = category;

		for (auto& element : elements)
		{
			const auto& [key, value] = element;
			auto sval = SValue();

			if (is::anInt(value))
			{
				sval.type = SType::INT;
				sval.value.i = std::stoi(value);
			}
			else if (is::aFloat(value))
			{
				sval.type = SType::FLOAT;
				sval.value.f = std::stof(value);
			}
			else
			{
				sval.type = SType::STRING;
				sval.value.s = value;
			}

			this->settings.insert({ key, sval });
		}
	}
}

SValue SettingsManager::getData(std::string name, SType type)
{
	SValue setting;

	try
	{
		setting = this->settings.at(name);
		if (setting.type != type)
		{
			LOG_F(ERROR, "Invalid type when parsing setting '%s'. You should verify the template type of the internal call.", name.c_str());
			setting.type = SType::INVALID;
		}
	}
	catch (const std::out_of_range&)
	{
		LOG_F(ERROR, "Couldn't find setting with name '%s'.", name.c_str());
		setting.type = SType::INVALID;
	}

	return setting;
}