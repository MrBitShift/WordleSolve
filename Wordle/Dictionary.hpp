#pragma once

#include <string>
#include <fstream>
#include <ios>
#include <stdexcept>

#include <vector>

#include <iostream>

class Dictionary
{
private:
	std::ifstream dict;

public:
	std::vector<std::string> values;

	Dictionary() = default;

	Dictionary& operator =(Dictionary& right)
	{
		values = right.values;
		return *this;
	}

	Dictionary(const Dictionary& d)
	{
		values = d.values;
	}

	Dictionary(std::string pathName, size_t wordleLength)
	{
		dict.open(pathName, std::ifstream::in);
		if (!dict.is_open())
			throw std::ios_base::failure("Could not open " + pathName);

		std::string line;
		while (std::getline(dict, line))
			if (line.size() == wordleLength)
				values.push_back(line);

		dict.close();
	}
};