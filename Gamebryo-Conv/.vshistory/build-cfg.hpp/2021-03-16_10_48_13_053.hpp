#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>

class Config {
	INI* _config{ nullptr };

public:
	Config(INI& config_inst) : _config{ &config_inst } {}

	bool write_default(const std::string& filename = "def.ini")
	{
		const section_map 
			defMap{
				{
					"conversions", { 
						{ "imperial_to_metric",	"0.3048" }, 
						{ "metric_to_unit",		"0.0142875313" }, 
						{ "imperial_to_unit",	"21.3333333333" }, 
					}
				},
			},
			defCommentMap{
				{
					"conversions", {
						{ "imperial_to_metric",	"Used for conversions between metric & imperial. ([Metric] = [Imperial] * [Factor])" },
						{ "metric_to_unit",		"" },
						{ "imperial_to_unit",	"" },
					}
				},
			};
		return INI_Defaults{ defMap, defCommentMap }.write(filename);
	}

	std::string get(std::string&& header, std::string&& key)
	{
		return _config->get(std::forward<std::string>(header), std::forward<std::string>(key)).value_or("");
	}
	INI::key_list* get(std::string&& header)
	{
		return _config->getSecPtr(std::forward<std::string>(header));
	}

	friend std::ostream& operator<<(std::ostream& os, const Config& c)
	{
		
		return os;
	}
};