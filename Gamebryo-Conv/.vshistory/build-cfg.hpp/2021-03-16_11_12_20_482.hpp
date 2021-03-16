#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>

/**
 * write_default_ini(const string&)
 * @brief Write the default INI config values to disk.
 * @param filename	- (Default: "def.ini")
 * @return true		- Successfully wrote default INI values to disk.
 * @return false	- Failed to write default INI values to disk.
 */
bool write_default_ini(const std::string& filename = "def.ini", bool overwrite = true)
{
	const section_map
		// Default Setting Values
		defMap{
			{
				"conversions", {
					{ "imperial_to_metric",	"0.3048" },
					{ "metric_to_unit",		"0.0142875313" },
					{ "imperial_to_unit",	"21.3333333333" },
				}
			},
			{
				"config", {
					{ "disable-colors", "false" },
					{ "always-show-input", "false" },
				}
			},
		},
		// Default Setting Comments
		defCommentMap{
			{
				"conversions", {
					{ "imperial_to_metric",	"Used for conversions between metric & imperial. ([Metric] = [Imperial] * [Factor])" },
					{ "metric_to_unit",		"Used for conversions between metric & units. ([Units] = [Metric] * [Factor])" },
					{ "imperial_to_unit",	"Used for conversions between imperial & units. ([Units] = [Imperial] * [Factor])" },
				}
			},
			{
				"config", {
					{ "disable-colors", "false" },
					{ "always-show-input", "false" },
				}
			},
		};
	// forward the return value from the INI_Defaults::write() function.
	return INI_Defaults{ defMap, defCommentMap }.write(filename, !overwrite);
}

class Config {
	INI* _config{ nullptr };

public:
	Config() = delete;

	/**
	 * Config(INI&)
	 * @brief Constructor that takes a reference to an INI instance.
	 * @param config_inst	- Reference to an INI instance.
	 */
	explicit Config(INI& config_inst) noexcept : _config{ &config_inst } {}

	/**
	 * get(string&&, string&&)
	 * @brief Get the value of a specified INI config value.
	 */
	std::string get(std::string&& header, std::string&& key)
	{
		if (_config != nullptr) return _config->get(std::forward<std::string>(header), std::forward<std::string>(key)).value_or("");
		throw std::exception("CONFIG_INI_NULLPTR");
	}

	/**
	 * get(string&&)
	 * @brief Get a pointer to a specified header in the INI file.
	 * @param header	- Target INI header.
	 * @returns INI::key_list*
	 * @throws exception("CONFIG_INI_NULLPTR")	- The local INI instance pointer was nullptr.
	 */
	INI::key_list* get(std::string&& header)
	{
		if (_config != nullptr) return _config->getSecPtr(std::forward<std::string>(header));
		throw std::exception("CONFIG_INI_NULLPTR");
	}

	/**
	 * operator<<(ostream&, const Config&)
	 * @brief Stream insertion operator.
	 * @param os	- (Auto) Target output stream reference.
	 * @param c		- (Auto) Target config reference.
	 * @returns ostream&
	 */
	friend std::ostream& operator<<(std::ostream& os, const Config& c) noexcept
	{
		os << c._config;
		return os;
	}
};