#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>
#include <cctype>
#include "logger/loguru.hpp"

class is
{
public:
	/**
	 * @brief Verifies if the passed-in parameter is a float.
	 * 
	 * @param data A string containing the value to check
	 * @return 'true' if float, 'false' if not.
	*/
	static bool aFloat(std::string data)
	{
		std::istringstream iss(data);
		float f;
		iss >> std::noskipws >> f;
		return iss.eof() && !iss.fail();
	}

	/**
	 * @brief Verifies if the passed-in parameter is an int.
	 * 
	 * @param data A string containing the value to check
	 * @return 'true' if int, 'false' if not.
	*/
	static bool anInt(std::string data)
	{
		if (data.empty() || (!isdigit(data[0]) && (data[0] != '-') && (data[0] != '+'))) return false;
		char* p;
		static_cast<void>(strtol(data.c_str(), &p, 10));
		return (*p == 0);
	}
};

class util
{
public:
	/**
	 * @brief Divides a string into a std::vector of substrings (fragments) depending on another sequence of characters.
	 * 
	 * @param string	The string to subdivide into fragments
	 * @param delimiter The sequence of characters by which this method should filter. If left undefined a whitespace is used
	 * @return A std::vector containing all string fragments that were separated by the delimiter.
	*/
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
	
	/**
	 * @brief Limits a double-precision floating point number to an upper and lower boundary.
	 * 
	 * @param n The number to clamp
	 * @param lower The lower boundary of the interval
	 * @param upper The upper boundary of the interval
	 * @return The interval boundary corresponding to the side on which 'n' is surpassing the interval, otherwise 'n'.
	*/
	static double clamp(double n, double lower, double upper)
	{
		if (n < lower) return lower;
		else if (n > upper) return upper;
		return n;
	}

	/**
	 * @brief Do not use unless you know what you're doing.
	 * You probably meant to call one of the template versions (i.e., floatify<...>(...)) instead.
	*/
	template <typename T>
	static T floatify(std::vector<std::string>&)
	{
		static_assert(false);
		return T();
	}

	/**
	 * @brief Changes all strings in a std::vector into a glm::vec2 of floats.
	 * This operation will fail silently for all strings with index > 1, so make sure to only pass in a vector of length = 2.
	 * 
	 * @param strings The std::vector containing the strings to floatify
	 * @return A glm::vec2 containing floats extracted from the passed in string vector.
	*/
	template<>
	static glm::vec2 floatify<glm::vec2>(std::vector<std::string>& strings)
	{
		glm::vec2 res{ };
		res.x = (float) ::atof(strings[0].c_str());
		res.y = (float) ::atof(strings[1].c_str());
		return res;
	}

	/**
	 * @brief Changes all strings in a std::vector into a glm::vec3 of floats.
	 * This operation will fail silently for all strings with index > 2, so make sure to only pass in a vector of length = 3.
	 * 
	 * @param strings The std::vector containing the strings to floatify
	 * @return A glm::vec3 containing floats extracted from the passed in string vector.
	*/
	template<>
	static glm::vec3 floatify<glm::vec3>(std::vector<std::string>& strings)
	{
		glm::vec3 res{ };
		res.x = (float) ::atof(strings[0].c_str());
		res.y = (float) ::atof(strings[1].c_str());
		res.z = (float) ::atof(strings[2].c_str());
		return res;
	}

	/**
	 * @brief Finds the relative path of a file given the folder name and the type ending of the file
	 * 
	 * @param folder_name The name of the folder to search in
	 * @param type_ending The type ending of the file (case sensitive!)
	 * @return A string containing the relative path of the file, otherwise an empty string
	*/
	static std::string findPath(const std::string& folder_name, const std::string& type_ending)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folder_name))
		{
			if (entry.path().has_extension() && entry.path().extension().string().compare(type_ending) == 0) // spaghetti codebonara
			{
				return entry.path().relative_path().string();
			}
		}

		return "";
	};
};