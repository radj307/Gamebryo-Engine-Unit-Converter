#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>

class Config {

public:
	Config() {}

	friend std::ostream& operator<<(std::ostream& os, const Config& c)
	{

	}
};