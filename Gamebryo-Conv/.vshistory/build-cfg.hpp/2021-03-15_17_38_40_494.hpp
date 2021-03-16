#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>

class Config {
	INI config;

public:
	Config(const int argc, char* argv[]) {}

	friend std::ostream& operator<<(std::ostream& os, const Config& c)
	{
		
		return os;
	}
};