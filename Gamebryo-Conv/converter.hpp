#pragma once
#include <map>
#include <string>

static const inline std::map<std::string, unsigned int> _conversionMap{
	{ "m", 0 }, { "meter", 0 }, { "meters", 0 },
	{ "cm", 100 }, { "centimeter", 100 }, { "centimeters", 100 },
	{ "mm", 1000 }, { "millimeter", 1000 }, { "millimeters", 1000 },
	{ "um", 1000000 }, { "micrometer", 1000000 }, { "micrometers", 1000000 },
	{ "f", 0 }, { "ft", 0 }, { "foot", 0 }, { "feet", 0 },
	{ "i", 12 }, { "in", 12 }, { "inch", 12 }, { "inches", 12 },
	{ "u", 0 }, { "un", 0 }, { "unit", 0 }, { "units", 0 },
};

inline std::string parse(const std::tuple<std::string, std::string, std::string>& params)
{

	return{};
}