#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>

class Config {
	INI* _config{ nullptr };

public:
	Config(INI& config_inst) : _config{ &config_inst } {}

	std::string get(std::string&& header, std::string&& key)
	{
		return _config->get(std::forward<std::string>(header), std::forward<std::string>(key)).value_or("");
	}
	INI::key_list get(std::string&& header)
	{
		
	}

	friend std::ostream& operator<<(std::ostream& os, const Config& c)
	{
		
		return os;
	}
};