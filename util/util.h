#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <glm/glm.hpp>
#include <iostream>
#include "logger/loguru.hpp"

class is
{
public:
	static bool aFloat(std::string data) {
		std::istringstream iss(data);
		float f;
		iss >> std::noskipws >> f;
		return iss.eof() && !iss.fail();
	}

	static bool anInt(std::string data) {
		if (data.empty() || (!isdigit(data[0]) && (data[0] != '-') && (data[0] != '+'))) return false;
		char* p;
		static_cast<void>(strtol(data.c_str(), &p, 10));
		return (*p == 0);
	}
};

class util
{
public:
	static std::vector<std::string> split(std::string string, const std::string& delimiter = " ")
	{
		std::vector<std::string> fragments;

		std::size_t pos = 0;
		std::size_t delim_pos = string.find(delimiter);

		if (delim_pos == std::string::npos)
		{
			fragments.push_back(string);
			return fragments;
		}

		while (delim_pos != std::string::npos)
		{
			std::string token = string.substr(pos, delim_pos - pos);
			fragments.push_back(token);
			
			pos = delim_pos + delimiter.length();
			delim_pos = string.find(delimiter, pos);
		}
		
		fragments.push_back(string.substr(pos, string.length() - pos));

		return fragments;
	}

	template <typename T>
	static T floatify(std::vector<std::string>&)
	{
		return T();
	}

	template<>
	static glm::vec2 floatify<glm::vec2>(std::vector<std::string>& strings)
	{
		glm::vec2 res;
		res.x = ::atof(strings[0].c_str());
		res.y = ::atof(strings[1].c_str());
		return res;
	}

	template<>
	static glm::vec3 floatify<glm::vec3>(std::vector<std::string>& strings)
	{
		glm::vec3 res;
		res.x = ::atof(strings[0].c_str());
		res.y = ::atof(strings[1].c_str());
		res.z = ::atof(strings[2].c_str());
		return res;
	}

	static void LOG_ERROR_AND_EXIT(const char* message)
	{
		LOG_F(ERROR, message);
		exit(EXIT_FAILURE);
	}
};