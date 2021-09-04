#pragma once
#include <iostream>
#include <INI.hpp>
#include <INI_Defaults.hpp>
#include <strconv.hpp>

struct {
	const section_map
		// Default Setting Values
		defMap{
			{
				"conversions", {
					{ "imperial_to_metric",	"0.3048" },			// Factor_MI
					{ "metric_to_unit",		"0.0142875313" },	// Factor_UM
					{ "imperial_to_unit",	"0.046875" },	// Factor_UI
				}
			},
			{
				"config", {
					{ "precision", "6" },
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

	bool write(const std::string& filename, bool append = false)
	{
		return INI_Defaults{ defMap, defCommentMap }.write(filename, append);
	}

	INI buildINI()
	{
		return INI( std::forward<decltype(defMap)>(defMap) );
	}

} DEFAULT_INI;

/**
 * @class Config
 * @brief Wrapper for the INI class used for config values throughout the conversion program.
 */
class Config {
	INI* _config{ nullptr }; ///< @brief Pointer to an INI instance, used by all member functions to retrieve values.

public:
	/**
	 * Config() = delete;
	 * @brief Deleted default constructor.
	 */
	Config() = delete;

	/**
	 * Config(INI&)
	 * @brief Constructor that takes a reference to an INI instance.
	 * @param config_inst	- Reference to an INI instance.
	 */
	explicit Config(INI& config_inst) noexcept : _config{ &config_inst } {}

	/**
	 * get(string&&)
	 * @brief Get a pointer to a specified header in the INI file.
	 * @param header	- Target INI header.
	 * @returns INI::key_list*
	 * @throws exception("CONFIG_INI_NULLPTR")	- The local INI instance pointer was nullptr.
	 */
	INI::key_list* get(std::string&& header)
	{
		if ( _config != nullptr ) return _config->getSecPtr(std::forward<std::string>(header));
		throw std::exception("CONFIG_INI_NULLPTR");
	}

	/**
	 * get(string&&, string&&)
	 * @brief Get the value of a specified INI config value.
	 * @param header	- The name of the header containing the target key.
	 * @param key		- The name of the target key.
	 * @returns string
	 * @throws exception("CONFIG_INI_NULLPTR")	- The local INI instance pointer was nullptr.
	 */
	std::string get(std::string&& header, std::string&& key)
	{
		if ( _config != nullptr ) return _config->get(std::forward<std::string>(header), std::forward<std::string>(key)).value_or("");
		throw std::exception("CONFIG_INI_NULLPTR");
	}

	/**
	 * iGet(string&&, string&&)
	 * @brief Get the value of a specified INI config value, as a long.
	 * @param header	- The name of the header containing the target key.
	 * @param key		- The name of the target key.
	 * @returns long
	 */
	long iGet(std::string&& header, std::string&& key)
	{
		return str::stol(get(std::forward<std::string>(header), std::forward<std::string>(key)));
	}

	/**
	 * fGet(string&&, string&&)
	 * @brief Get the value of a specified INI config value, as a double.
	 * @param header	- The name of the header containing the target key.
	 * @param key		- The name of the target key.
	 * @returns double
	 */
	double fGet(std::string&& header, std::string&& key)
	{
		return str::stod(get(std::forward<std::string>(header), std::forward<std::string>(key)));
	}

	/**
	 * bGet(string&&, string&&)
	 * @brief Get the value of a specified INI config value, as a boolean.
	 * @param header	- The name of the header containing the target key.
	 * @param key		- The name of the target key.
	 * @returns bool
	 */
	bool bGet(std::string&& header, std::string&& key)
	{
		return str::stob(get(std::forward<std::string>(header), std::forward<std::string>(key)));
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
Config* cfg{ nullptr }; ///< @brief This is required until I go through the whole program and change the use of global vars.
