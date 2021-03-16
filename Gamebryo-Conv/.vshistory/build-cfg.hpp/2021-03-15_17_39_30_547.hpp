#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>

class Config {
	INI _config;

public:
	Config(const int argc, char&& argv[]) : _config{argc, std::forward<char>(argv)} {}

	friend std::ostream& operator<<(std::ostream& os, const Config& c)
	{
		
		return os;
	}
};